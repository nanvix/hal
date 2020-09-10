/*
 * MIT License
 *
 * Copyright(c) 2011-2020 The Maintainers of Nanvix
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _VSYSCALL_H_
#define _VSYSCALL_H_

	/**
	 * @brief Number of system calls.
	 *
	 * @note This should be set to the highest system call number.
	 */
	#define NR_SYSCALLS NR_last_kcall

	/**
	 * @name System Call Numbers
	 */
	/**@{*/
	#define NR_exit             0 /**< exit()            */
	#define NR_sync_create      1 /**< sync_create()     */
	#define NR_sync_open        2 /**< sync_open()       */
	#define NR_sync_unlink      3 /**< sync_unlink()     */
	#define NR_sync_close       4 /**< sync_close()      */
	#define NR_sync_wait        5 /**< sync_wait()       */
	#define NR_sync_signal      6 /**< sync_signal()     */
	#define NR_mailbox_create   7 /**< mailbox_create()  */
	#define NR_mailbox_open     8 /**< mailbox_open()    */
	#define NR_mailbox_unlink   9 /**< mailbox_unlink()  */
	#define NR_mailbox_close   10 /**< mailbox_close()   */
	#define NR_mailbox_awrite  11 /**< mailbox_awrite()  */
	#define NR_mailbox_aread   12 /**< mailbox_aread()   */
	#define NR_mailbox_wait    13 /**< mailbox_wait()    */
	#define NR_portal_create   14 /**< portal_create()   */
	#define NR_portal_allow    15 /**< portal_allow()    */
	#define NR_portal_open     16 /**< portal_open()     */
	#define NR_portal_unlink   17 /**< portal_unlink()   */
	#define NR_portal_close    18 /**< portal_close()    */
	#define NR_portal_awrite   19 /**< portal_awrite()   */
	#define NR_portal_aread    20 /**< portal_aread()    */
	#define NR_portal_wait     21 /**< portal_wait()     */

	#define NR_last_kcall      23 /**< NR_SYSCALLS definer      */
	/**@}*/

/*============================================================================*
 * Sysboard Calls                                                             *
 *============================================================================*/


	EXTERN void vsys_loop(void);
	EXTERN void vsys_setup(void);
	EXTERN void vsys_exit(void);

/*============================================================================*
 * Sync Kernel Calls                                                          *
 *============================================================================*/

	EXTERN int vsys_sync_create(const int *, int, int);
	EXTERN int vsys_sync_open(const int *, int, int);
	EXTERN int vsys_sync_unlink(int);
	EXTERN int vsys_sync_close(int);
	EXTERN int vsys_sync_wait(int);
	EXTERN int vsys_sync_signal(int);

/*============================================================================*
 * Mailbox Kernel Calls                                                       *
 *============================================================================*/

	EXTERN int vsys_mailbox_create(int);
	EXTERN int vsys_mailbox_open(int);
	EXTERN int vsys_mailbox_unlink(int);
	EXTERN int vsys_mailbox_close(int);
	EXTERN int vsys_mailbox_aread(int, void *, size_t);
	EXTERN int vsys_mailbox_awrite(int, const void *, size_t);
	EXTERN int vsys_mailbox_wait(int);

/*============================================================================*
 * Portal Kernel Calls                                                        *
 *============================================================================*/

	EXTERN int vsys_portal_create(int);
	EXTERN int vsys_portal_allow(int, int);
	EXTERN int vsys_portal_open(int, int);
	EXTERN int vsys_portal_unlink(int);
	EXTERN int vsys_portal_close(int);
	EXTERN int vsys_portal_aread(int, void *, size_t);
	EXTERN int vsys_portal_awrite(int, const void *, size_t);
	EXTERN int vsys_portal_wait(int);

#endif /* _VSYSCALL_H_ */
