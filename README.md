# QThreadPlayground

This is a small repository to play around with different QThread parts.

The goal was to put some "function" on its own thread to not block the user interface.

Use the button to launch some workers and click the other button to see that the GUI thread is not blocked (the text box below should change colour).


# Controller & Worker

The controller class makes a thread and a worker, moves the worker to the tread and connections up the signals.

When the worker is done, it signals back to the Controller, which then processes the results and cleans up.

I'm using a single shot connection between the worker and controller, because otherwise multiple workers would be connected to the same signal. Once the next worker was created and the operate signal was emitted, all previous workers would also receive that signal, but in my case that is not required.


# Log all events filter

Using the Qt meta object system I'm logging all events received, and if it's a meta call (slot invocation), the arguments and sender are also logged. 

Useful when troubleshooting threading issues.
