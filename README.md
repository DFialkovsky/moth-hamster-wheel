moth-hamster-wheel
==================
GUI Program in Qt that reads a text fine emitted from an embedded system (currently an arduino) connected to a flight mill. The flight mill records a time stamp every time a full rotation was made. from that data, the moth-hamster-wheel will parse that data into more useful information

Currently a work in progress. 

How it works:

See example.csv for an example of proper file format (ID,[millisecond timestamp])

This will emit a test.xls file with data regarding bouts (per 60 seconds) and some more statistics.

I am deciding whether to emit a timestamped file automaticlly, or have the user specify it.
