Lab 2 Documentation
Isaac Fisch ifisch
Nick Palumbo

We created a thread that makes the egg timer run when once it has been started.

ledset {LED} {STATE}
Our ledset function takes an led (N, NE, E, SE, S, SW, W, or NW) and a state
(on or off). It changes the selected led to the the selected state.

ledread {LED}
The ledread funtion just takes an led (N, NE, E, SE, S, SW, W, or NW). It
prints the current state of the selected led.

timerset {TIME}
The timerset function takes a time from 10-10000 in milliseconds. It sets the
egg timer to the entered time.

timerreset
The timerreset function resets the egg timer to whatever number of milliseconds
it had most previously been set to.

timerstart
The timerstart function starts running the egg timer.

timerstop
The timerstop function stops running the egg timer.

timergettime
The timergettime function prints out the number of milliseconds left on the
egg timer.


