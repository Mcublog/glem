/******************************************************************************

                  Copyright (c) 2017 Siddharth Chandrasekaran

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.

******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <glem.h>

/*
 * Demo: partial frame updates (line-by-line writes).
 *
 * The glem_write() call sends the whole frame buffer and the server rejects
 * anything shorter than the full frame. To write line-by-line, use
 * glem_write_at() with the byte offset of the line within the frame buffer.
 * For a row-major display the offset of line y is y * (width / 8).
 */

int main(int argc, char *argv[])
{
	int w, h;
	uint8_t *line;
	int line_len, y;

	if (argc != 3) {
		printf("Usage: %s <width> <height>\n", argv[0]);
		exit(-1);
	}
	w = atoi(argv[1]);
	h = atoi(argv[2]);

	glem_init(w, h, 0);

	line_len = w / 8;
	line = calloc(line_len, sizeof(uint8_t));
	if (line == NULL) {
		printf("Out of memory\n");
		exit(-1);
	}

	struct timespec t0, t1;
	clock_gettime(CLOCK_MONOTONIC, &t0);
	for (y = 0; y < h; y++) {
		memset(line, 0x00, line_len);
		/* Border: solid first and last line. */
		if (y == 0 || y == h - 1) {
			memset(line, 0xFF, line_len);
		}
		glem_write_at(line, line_len, y * line_len);
	}
	clock_gettime(CLOCK_MONOTONIC, &t1);

	printf("wrote %d lines (%d bytes each) in %.2f ms\n",
			h, line_len,
			((t1.tv_sec - t0.tv_sec) * 1000.0) +
			((t1.tv_nsec - t0.tv_nsec) / 1000000.0));

	free(line);
	return 0;
}
