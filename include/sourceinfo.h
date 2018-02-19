/*
 * Copyright (C) 2006 Atheme Development Group
 * Rights to this code are as documented in doc/LICENSE.
 *
 * Data structures for sourceinfo
 */

#ifndef SOURCEINFO_H
#define SOURCEINFO_H

struct sourceinfo_vtable
{
	const char *description;
	const char *(*format)(struct sourceinfo *si, bool full);
	void (*cmd_fail)(struct sourceinfo *si, enum cmd_faultcode code, const char *message);
	void (*cmd_success_nodata)(struct sourceinfo *si, const char *message);
	void (*cmd_success_string)(struct sourceinfo *si, const char *result, const char *message);
	void (*cmd_success_table)(struct sourceinfo *si, struct atheme_table *table);
	const char *(*get_source_name)(struct sourceinfo *si);
	const char *(*get_source_mask)(struct sourceinfo *si);
	const char *(*get_oper_name)(struct sourceinfo *si);
	const char *(*get_storage_oper_name)(struct sourceinfo *si);
};

/* structure describing data about a protocol message or service command */
struct sourceinfo
{
	struct atheme_object parent;

	/* fields describing the source of the message */
	/* for protocol modules, the following applies to su and s:
	 * at most one of these two can be non-NULL
	 * before server registration, both are NULL, otherwise exactly
	 * one is NULL.
	 * for services commands, s is always NULL and su is non-NULL if
	 * and only if the command was received via IRC.
	 */
	struct user *su; /* source, if it's a user */
	struct server *s; /* source, if it's a server */

	struct connection *connection; /* physical connection cmd received from */
	const char *sourcedesc; /* additional information (e.g. IP address) */
	myuser_t *smu; /* login associated with source */

	/* the service the original command was sent to, which may differ
	 * from the service the current command is in
	 */
	struct service *service;

	struct channel *c; /* channel this command applies to (fantasy?) */

	struct sourceinfo_vtable *v; /* function pointers, could be NULL */
	void *callerdata; /* opaque data pointer for caller */

	unsigned int output_limit; /* if not 0, limit lines of output */
	unsigned int output_count; /* lines of output upto now */

	struct language *force_language; /* locale to force replies to be in, could be NULL */

	struct command *command; /* The command being executed. Needed when one function handles
				more than one command. */
};

#endif
