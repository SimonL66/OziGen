#include <stdio.h>
#include <stdlib.h>
#include <memory.h>

static unsigned char memory[8192];
static unsigned char * compressed;
static unsigned char * uncompressed;
static long uncompressed_length;

void decodePositive(void)
{
int mask;			// 0x0f
int nbits;			// 0x10
unsigned char * compressed_ptr;
int nbytes;
char value;

	value = *compressed;
	compressed_ptr = 1 + compressed + value;		//esi
	nbits = 0;
	if (value <= 1) {
		mask = 0;
	} else {
		nbits = 0;
		do {
			nbits++;
			mask = 1<<nbits;
		} while (value > mask);
		mask--;
	}

	do {
		nbytes = (unsigned char)*compressed_ptr++;
		value = compressed[1 + (nbytes & mask)];
		nbytes >>= nbits;
		uncompressed_length -= nbytes;
		while (nbytes-- > 0)
			*uncompressed++ = value;
	} while (uncompressed_length > 0);
}


/*
0			[nColours]			Number of colours in palette
1			[palette]			Palette
1+nColours	[Compressed data]

For 1	colours: [76543210] = [76543210] copies of colour index 0
For 2	colours: [76543210] = [7654321] copies of colour [0]
For 3	colours: [76543210] = [765432] copies of colour [10]
For 7	colours: [76543210] = [76543] copies of colour [210]
For 15	colours: [76543210] = [7654] copies of colour [3210]
For 31	colours: [76543210] = [765] copies of colour [43210]
For 63	colours: [76543210] = [76] copies of colour [543210]
For 127 colours: [76543210] = [7] copies of colour [6543210]

*/

/*
00			00					Invariant

00..7f
if 80, xx, yy	compressed_ptr += 3 - (yy<<8 + xx)
if <0,			compressed_ptr += 1 - [value]


00 xx							0 <= xx <= 0x7f		Copy xx for whole tile

00 xx yy						0x80 <= xx <= 0xFF	Set 

00
[huffman tree]
bits to select bytes from huffman tree
*/
void decode0(void)
{
unsigned char * compressed_ptr;
char value;
int src_byte;
unsigned char * src_ptr;

	compressed_ptr = (unsigned char*)(compressed+1);
	while ((value = compressed_ptr[0]) < 0) {
		if (value == -128) {
			compressed_ptr += 2 + 1 - (short)((compressed_ptr[2] << 8) + compressed_ptr[1]);
		} else
			compressed_ptr += 1 - value;
	}
	src_byte = 0;
	compressed_ptr++;
	while (uncompressed_length-- > 0) {
		src_ptr = compressed+1;
		while ((value = *src_ptr) < 0) {
			if ((src_byte & 0x100) == 0)
				src_byte = *compressed_ptr++ | 0xff00;
			if (value == -128) {
				if (src_byte&1)
					src_ptr -= (short)(((src_ptr[2]) << 8) + src_ptr[1]);
				src_ptr += 3;
			} else {
				if (src_byte&1)
					src_ptr -= value;
				src_ptr++;
			}
			src_byte >>= 1;	
		}
		*uncompressed++ = value;
	}
}

long compress(unsigned char * data, unsigned char * output)
{
long nColours = 0;
unsigned char colours[256];
unsigned char colour;
long index;
long len_output;
long nBits, maxCopies, nCount;

	// Build reduced colour palette
	memset(colours, 0xff, sizeof(colours));
	for (index = 0; index<4096; index++) {
		colour = data[index];
		if (colours[colour] == 0xff) {
			output[1+nColours] = colour;
			colours[colour] = (unsigned char)nColours++;
		}
	}

	nBits = 0;
	while ((1<<nBits) < nColours)
		nBits++;
	maxCopies = (1<<(8-nBits))-1;

	output[0] = (unsigned char)nColours;
	len_output = 1 + nColours;
	colour = data[0];
	nCount = 1;

	for (index = 1; index<4096; index++) {
		if (colour != data[index] || nCount == maxCopies) {
			output[len_output++] = ((nCount)<<nBits) + colours[colour];
			colour = data[index];
			nCount = 1;
		} else
			nCount++;
	}
	output[len_output++] = ((nCount)<<nBits) + colours[colour];
	return len_output;
}

void pqdownheap(long N, long * counts, long * heap, long k)
{
long j, v;

	v = heap[k];
	while (k <= N/2) {
		j = k+k;
		if (j<N && counts[heap[j]] > counts[heap[j+1]])
			j++;
		if (counts[v] < counts[heap[j]])
			break;
		heap[k] = heap[j];
		k = j;
	}
	heap[k] = v;
}

unsigned char g_output[1024];
long output_ptr = 1024;

void dumptree_uw(long children[][2], long index)
{
long offset_to_next;
	if (children[index][0] > 256) {
		dumptree_uw(children, children[index][0]-256);
	} else {
		g_output[--output_ptr] = (unsigned char)(children[index][0]);
	}
	offset_to_next = output_ptr;
	if (children[index][1] > 256) {
		dumptree_uw(children, children[index][1]-256);
	} else {
		g_output[--output_ptr] = (unsigned char)(children[index][1]);
	}
	offset_to_next = output_ptr - offset_to_next;
	if (offset_to_next < -127) {
		g_output[--output_ptr] = (unsigned char)(offset_to_next >> 8);
		g_output[--output_ptr] = (unsigned char)(offset_to_next & 0xff);
		g_output[--output_ptr] = (unsigned char)(0x80);
	} else {
		g_output[--output_ptr] = (unsigned char)(offset_to_next & 0xff);
	}
}

void dumptree(long children[][2], long index)
{
	if (children[index][1] > 256) {
		dumptree(children, children[index][1]-256);
	} else {
		printf("%d\n", children[index][1]);
	}
	if (children[index][0] > 256) {
		dumptree(children, children[index][0]-256);
	} else {
		printf("%d\n", children[index][0]);
	}
}


long compressHuffman(unsigned char * data, unsigned char * output)
{
long dad[514];
long counts[514];
long heap[514];
long N;
long index;
long t;
long codes[256], lengths[256];
long children[256][2];

	// Determine frequencies of data items
	memset(counts, 0, sizeof(counts));
	for (index = 0; index<4096; index++)
		counts[data[index]]++;

	// build heaps
	N = 0;
	for (index = 0; index<256; index++) {
		if (counts[index] != 0) {
			heap[++N] = index;
		}
	}
	if (N==1) {
		// One colour for whole tile
		output[0] = 0;
		output[1] = (unsigned char)(heap[1]);
		return 2;
	}

	for (index = N; index >= 1; index--)
		pqdownheap(N, counts, heap, index);

	do {
		t = heap[1];
		heap[1] = heap[N];
		N--;
		pqdownheap(N, counts, heap, 1);
		counts[256+N] = counts[heap[1]] + counts[t];
		children[N][0] = heap[1];
		children[N][1] = t;
		dad[t] = 256+N;
		dad[heap[1]] = -256-N;
		heap[1] = 256+N;
		pqdownheap(N, counts, heap, 1);
	} while (N > 1);
	dad[256+N] = 0;

	for (index = 0; index<256; index++) {
		if (counts[index] != 0) {
			long nBits = 0, code = 0;
			t = dad[index];
			do {
				code <<= 1;
				if (t < 0) {
					code++;
					t = -t;
				}
				t = dad[t];
				nBits++;
			} while (t != 0);
			codes[index] = code<<(30-nBits);
			lengths[index] = nBits;
//			printf("%d %d %d\n", index, codes[index]>>(24-nBits), nBits);
		}
	}
/*
value/
0 1 2
1 3 2
2 2 2
3 0 2
*/
//	dumptree(children, 1);
	output_ptr = 1024;
	dumptree_uw(children, 1);

	{
	long length = 0;
	long outBits = 0;
	long outByte = 0;
	long nBits;

	output[0] = 0;
	memcpy(output+1, g_output+output_ptr, 1024-output_ptr);
	length = 1 + 1024-output_ptr;
	for (index = 0; index<4096; index++) {
		nBits = lengths[data[index]];
		outByte = (outByte >> nBits) + codes[data[index]];
		outBits += nBits;
		while (outBits >= 8) {
			output[length++] = (unsigned char)(outByte >> (30-outBits));
			outBits -= 8;
		}
	}
	if (outBits != 0) {
		output[length++] = (unsigned char)(outByte >> (30-outBits));
	}
	return length;
	}
}


unsigned char buffer_in[4096];
unsigned char buffer_out[8192];

void compress_test(void)
{
long nColours;
long index;
long length;
FILE * fp;
	for (nColours = 1; nColours<129; nColours++) {
		for (index = 0; index<4096; index++) {
			buffer_in[index] = (unsigned char)(rand()%nColours);
		}
		if (nColours == 128) {
			memset(buffer_in, 0, sizeof(buffer_in));
			fp = fopen("compress.cpp", "r");
			fread(buffer_in, 1, 4096, fp);
			fclose(fp);
		}
		length = compress(buffer_in, buffer_out);
		printf("%d=%d ", nColours, length);

		memset(memory, 0xaa, sizeof(memory));
		uncompressed = memory;
		compressed = buffer_out;
		uncompressed_length = 4096;
		decodePositive();
		if (memcmp(memory, buffer_in, 4096) != 0 ||
			memory[4096] != 0xaa ||
			memory[4097] != 0xaa ||
			memory[4098] != 0xaa)
		{
			printf("Error in encoder/decoder\n");
		}

		length = compressHuffman(buffer_in, buffer_out);
		printf("%d\n", length);
		memset(memory, 0xaa, sizeof(memory));
		uncompressed = memory;
		compressed = buffer_out;
		uncompressed_length = 4096;
		decode0();
		if (memcmp(memory, buffer_in, 4096) != 0 ||
			memory[4096] != 0xaa ||
			memory[4097] != 0xaa ||
			memory[4098] != 0xaa)
		{
			printf("Error in encoder/decoder\n");
		}

	}
}

long compressHuffmanTest(unsigned char * data, unsigned char * output)
{
long length;
	length = compressHuffman(data, output);
//	printf("%d\n", length);
	memset(memory, 0xaa, sizeof(memory));
	uncompressed = memory;
	compressed = buffer_out;
	uncompressed_length = 4096;
	memcpy(compressed, output, length);
	decode0();
	if (memcmp(memory, data, 4096) != 0 ||
		memory[4096] != 0xaa ||
		memory[4097] != 0xaa ||
		memory[4098] != 0xaa)
	{
		long i;
		for (i=0; i<4096; i++) {
			if (memory[i] != data[i])
				break;
		}
		printf("Error in encoder/decoder\n");
	}
	return length;
}
