/*
 * Copyright (c) 2017 Linaro Limited
 * Copyright (c) 2018 Foundries.io
 *
 * SPDX-License-Identifier: Apache-2.0
 */

#ifndef ELCE2018_APP_WORK_QUEUE_H__
#define ELCE2018_APP_WORK_QUEUE_H__

/**
 * @file
 * @brief ELCE2018 application work queue
 *
 * This is a single work queue which can be used for sleeping tasks
 * within this application.
 *
 * Work handlers submitted to this queue may sleep or yield.
 *
 * Work may be submitted to this queue only by threads started from
 * main().
 */

#include <zephyr.h>
#include <zephyr/types.h>

/*
 * This is the work queue itself, which can be passed along to other
 * APIs which submit work.
 */
extern struct k_work_q *app_work_q;

/**
 * @brief Initialize the application work queue.
 *
 * Work may be submitted to the queue after this function returns. It
 * will not be processed until app_wq_run() is called.
 *
 * @see app_wq_run()
 */
void app_wq_init(void);

/**
 * @brief Start handling work queue events in the current thread.
 *
 * Unlike k_work_q_start(), this does not create a new thread;
 * instead, it runs in the caller's.
 *
 * @see k_work_q_start()
 */
FUNC_NORETURN
void app_wq_run(void);

/**
 * @brief Submit work to the application work queue thread.
 * @param work Work to submit
 * @see k_work_submit_to_queue()
 */
static inline void app_wq_submit(struct k_work *work)
{
	k_work_submit_to_queue(app_work_q, work);
}

/**
 * @brief Submit delayed work to the application work queue thread.
 * @param work     Work to submit
 * @param delay_ms Delay in milliseconds
 * @return k_delayed_work_submit_to_queue() return value.
 * @see k_delayed_work_submit_to_queue()
 */
static inline int app_wq_submit_delayed(struct k_delayed_work *work,
					s32_t delay_ms)
{
	return k_delayed_work_submit_to_queue(app_work_q, work, delay_ms);
}

#endif /* ELCE2018_APP_WORK_QUEUE_H__ */
