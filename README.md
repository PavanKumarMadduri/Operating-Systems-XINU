# Operating-Systems-XINU Kernel Programming Assignments

### PA0: Getting Acquainted with XINU

Learned about the process of compiling and running XINU, the tools involved, and the run-time environment and segment layout.

### PA1: Process Scheduling

Implemented two new scheduling policies to overcome the starvation problem present in
the default XINU scheduler. Aging Based Scheduler, which increases the priority of all the processes in the ready
queue whenever a reschedule is called and the Linux Based Scheduler, which prioritizes processes using priority,
quantum left, and goodness.

### PA2: Readers/Writer Locks

Implemented Read/Write locks in XINU OS to synchronize
access to a shared data structure, overcoming the shortcomings of existing semaphore implementation. Also
addressed the Priority Inheritance problem by implementing the Priority Inheritance Protocol.

### PA3: Demand Paging

Implemented two-level paging mechanism which manages the memory by mapping a logical
address to physical memory using a page table. Emulated backing store to hold the pages for swapping in and out.
Implemented FIFO and SC page replacement policies.
