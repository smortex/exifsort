/*-
 * Copyright (c) 2012 Romain Tartière <romain@blogreen.org>.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <err.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include <libexif/exif-data.h>

struct photo {
    char *filename;
    time_t datetime;
};

int
compare_datetime (const void *r_left, const void *r_right)
{
    struct photo *left = (struct photo *) r_left;
    struct photo *right = (struct photo *) r_right;

    return difftime (left->datetime, right->datetime);
}

int
main (int argc, char *argv[])
{
    ExifData *ed;
    ExifEntry *entry;

    struct photo *photos;

    int n_photos = argc - 1;

    if (!(photos = malloc (n_photos * sizeof (*photos)))) {
	err (EXIT_FAILURE, "malloc");
    }

    for (int i = 1; i < argc; i++) {
	photos[i-1].filename = argv[i];
	photos[i-1].datetime = 0;

	if (!(ed = exif_data_new_from_file (argv[i]))) {
	    warn ("%s: can't read EXIF data", argv[i]);
	    continue;
	}

	entry = exif_content_get_entry (ed->ifd[EXIF_IFD_0], EXIF_TAG_DATE_TIME);

	char buf[BUFSIZ];
	exif_entry_get_value (entry, buf, sizeof (buf));

	struct tm time;
	strptime (buf, "%Y:%m:%d %H:%M:%S", &time);
	/*
	 * XXX Can we get the timezone settings from the EXIF info?
	 * My camera store the local time with no way to set the timezone.
	 * I should setup it UTC.
	 */
	photos[i-1].datetime = mktime (&time);

	exif_data_unref (ed);
    }

    qsort (photos, n_photos, sizeof (*photos), compare_datetime);

    for (int i = 0; i < n_photos; i++) {
	printf ("%s\n", photos[i].filename);
    }

    free (photos);

    exit (EXIT_SUCCESS);
}
