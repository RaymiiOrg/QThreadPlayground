# QThreadPlayground

This is a small repository to play around with different QThread parts.

The goal was to put some "function" on its own thread to not block the user interface.

The controller class makes a thread and a worker, moves the worker to the tread and connections up the signals.

When the worker is done, it signals back to the Controller, which then processes the results and cleans up.

I'm using a single shot connection between the worker and controller, because otherwise multiple workers would be connected to the same signal. Once the next worker was created and the operate signal was emitted, all previous workers would also receive that signal, but in my case that is not required.

