moth-hamster-wheel
==================
GUI Program in Qt that reads a text fine emitted from an embedded system (currently an arduino) connected to a flight mill. The flight mill records a time stamp every time a full rotation was made. from that data, the moth-hamster-wheel will parse that data into more useful information

Currently a work in progress. 

How it works:

A text file formatted in the following way
(\n implies a new line (return key) in case if the readme viewer is being funny)

34, 3\n
49, 3\n
1245, 3\n
2434 ,3\n
3234 ,5\n
3400, 5\n
3423, 3\n
...

This will emit a test.xls file with data regarding bouts (per 60 seconds) and some more statistics.

I am deciding whether to emit a timestamped file automaticlly, or have the user specify it.
