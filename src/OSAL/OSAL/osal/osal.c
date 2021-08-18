#include "osal.h"
#include "osal_event.h"
#include "osal_memory.h"

osal_msg_q_t osal_qHead;

/*********************************************************************
 * @fn      osal_init_system
 *
 * @brief
 *
 *   This function initializes the "task" system by creating the
 *   tasks defined in the task table (OSAL_Tasks.h).
 *
 * @param   void
 *
 * @return  ZSUCCESS
 */
uint8 osal_init_system( void )
{
  // Initialize the Memory Allocation System
  osal_mem_init();

  // Initialize the message queue
  osal_qHead = NULL;

#if defined( OSAL_TOTAL_MEM )
  osal_msg_cnt = 0;
#endif

  osalTimerInit();
  osal_init_TaskHead();  

  return ( ZSUCCESS );
}

/*********************************************************************
 * @fn      osal_start_system
 *
 * @brief
 *
 *   This function is the main loop function of the task system.  It
 *   will look through all task events and call the task_event_processor()
 *   function for the task with the event.  If there are no events (for
 *   all tasks), this function puts the processor into Sleep.
 *   This Function doesn't return.
 *
 * @param   void
 *
 * @return  none
 */
void osal_start_system( void )
{
  uint16 events;
  uint16 retEvents;

  while(1)
  {
    TaskActive = osalNextActiveTask();
    if ( TaskActive )
    {
      HAL_ENTER_CRITICAL_SECTION();
      events = TaskActive->events;
      // Clear the Events for this task
      TaskActive->events = 0;
      HAL_EXIT_CRITICAL_SECTION();

      if ( events != 0 )
      {
        // Call the task to process the event(s)
        if ( TaskActive->pfnEventProcessor )
        {
          retEvents = (TaskActive->pfnEventProcessor)( TaskActive->taskID, events );

          // Add back unprocessed events to the current task
          HAL_ENTER_CRITICAL_SECTION();
          TaskActive->events |= retEvents;
          HAL_EXIT_CRITICAL_SECTION();
        }
      }
    }
  }
}
