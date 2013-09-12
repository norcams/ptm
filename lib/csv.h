/* Copyright 2013 Cumulus Networks Inc.  All rights reserved. */
/* See License file for licenese. */

#ifndef __CSV_H__
#define __CSV_H__

/*
 * CSV encoding and decoding routines.
 *
 * Example:
 * Encoding side:
 *
 *      csv_t *csv;
 *      csv_record_t *fstrec;
 *      csv_record_t *rec;
 *      char  buf[BUFSIZ];
 *
 *      csv = csv_init(csv, buf, BUFSIZ);
 *      ...
 *      fstrec = csv_encode(csv, 2, "hello", "world");
 *      rec = csv_encode(csv, 2, "foo", "bar");
 *      ...
 *      fstrec = csv_encode_record(csv, fstrec, 2, "HELLO", "WORLD");
 *      ...
 *      csv_clean(csv);
 *
 * Decoding side:
 *
 *      csv_t *csv;
 *      csv_record_t *rec;
 *      csv_field_t *fld;
 *      char  *rcvdbuf;
 *
 *      csv = csv_init(csv, rcvdbuf, BUFSIZ);
 *      ...
 *      csv_decode(csv);
 *      csv_dump(csv);
 *
 *      for (rec = csv_record_iter(csv); rec;
 *           rec = csv_record_iter_next(rec)) {
 *          ...
 *          for (str = csv_field_iter(rec, &fld); str;
 *               str = csv_field_iter_next(&fld)) {
 *              ...
 *          }
 *      }
 *      ...
 *      csv_clean(csv);
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <sys/queue.h>

typedef struct _csv_field_t_ csv_field_t;
typedef struct _csv_record_t_ csv_record_t;
typedef struct _csv_t_ csv_t;

/**
 * Initialize the CSV structure (if necessary, allocate first). Point to
 * the passed string buffer.
 */
csv_t * csv_init(csv_t *csv, char *buf, int buflen);

/**
 * Encode the variable list of arguments as CSV fields. The csv structure
 * should have been initialized (with the string buffer). The fields get
 * concatenated into the string.
 */
csv_record_t *csv_encode(csv_t *csv, int count, ...);

/**
 * Encode the variable list arguments into an existing record, essentially
 * overwriting the record. No checking is done for consistency. The number
 * of fields should be the same as what was encoded and the length of each
 * field should also be the same as what was encoded before. The "rec"
 * parameter should be the same as what was returned from a previous call
 * to csv_encode().
 *
 * Useful for message encoding/decoding that get passed around between
 * processes/nodes - e.g. the message header record can be rewritten AFTER
 * encoding all other records, with new information such as total length.
 */
csv_record_t *csv_encode_record(csv_t *csv, csv_record_t *rec,
				int count, ...);

/**
 * Decode a CSV formatted string. The csv structure should have been
 * initialized (with the string). The function creates a LIST of records
 * (csv_record_t structure) where each record is in turn a LIST of fields
 * (csv_field_t structure). The record points to the string containing the
 * list of fields. Similarly, the field points to the field string.
 */
void csv_decode(csv_t *csv);

/**
 * Dump all fields of a decoded CSV to stderr
 */
void csv_dump(csv_t *csv);

/**
 * Total length of all characters encoded in the CSV.
 */
int csvlen(csv_t *csv);

void csv_clean(csv_t *csv);
void csv_free(csv_t *csv);

/**
 * Iterate through the records and fields of an encoded/decoded CSV.
 */
csv_record_t *csv_record_iter(csv_t *csv);
csv_record_t *csv_record_iter_next(csv_record_t *rec);
char *csv_field_iter(csv_record_t *rec, csv_field_t **fld);
char *csv_field_iter_next(csv_field_t **fld);

#endif
