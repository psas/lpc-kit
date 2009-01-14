

2008/06/24 11:04:55

The purpose of psas-lpc is to maintain a set of common
functions for all the sensor nodes built around the
LPC2148. 

These could be compiled as a library or the source could
be linked. 

There are two arguments for doing it this way:

1) Functions will have fewer errors if more people are looking at them.
2) Reduce duplication of effort
3) Increase consistency across sensor node software development.

There are arguments against doing it this way:
1) It's really uptight.
2) These functions don't work for application xxx...
3) You call that a makefile?


