/*
 * pg_wait_sampling.h
 *		Headers for pg_wait_sampling extension.
 *
 * Copyright (c) 2015-2016, Postgres Professional
 *
 * IDENTIFICATION
 *	  contrib/pg_stat_wait/pg_wait_sampling.h
 */
#ifndef __PG_WAIT_SAMPLING_H__
#define __PG_WAIT_SAMPLING_H__

#include <postgres.h>

#include "storage/proc.h"
#include "storage/shm_mq.h"
#include "utils/timestamp.h"

#define	PG_WAIT_SAMPLING_MAGIC		0xCA94B107
#define COLLECTOR_QUEUE_SIZE		(16 * 1024)
#define HISTORY_TIME_MULTIPLIER		10

typedef struct
{
	int				pid;
	uint32			wait_event_info;
	uint64			count;
} ProfileItem;

typedef struct
{
	int				pid;
	uint32			wait_event_info;
	TimestampTz		ts;
} HistoryItem;

typedef struct
{
	bool			wraparound;
	int				index;
	int				count;
	HistoryItem	   *items;
} History;

typedef enum
{
	NO_REQUEST,
	HISTORY_REQUEST,
	PROFILE_REQUEST,
	PROFILE_RESET
} SHMRequest;

typedef struct
{
	Latch		   *latch;
	SHMRequest		request;
	int				historySize;
	int				historyPeriod;
	int				profilePeriod;
} CollectorShmqHeader;

/* pg_stat_wait.c */
extern void check_shmem(void);
extern CollectorShmqHeader *collector_hdr;
extern shm_mq			   *collector_mq;

extern void read_current_wait(PGPROC *proc, HistoryItem *item);
extern void RegisterWaitsCollector(void);
extern void AllocHistory(History *, int);

#endif
