#!/usr/bin/python

animals = "catdogfrog"
cat = animals[0:3]# The first three characters [:3] of `animals`
dog = animals[3:6]# The fouth through sixth characters
frog = animals[6:]# From the seventh character to the end
print "animals list = " + str(animals)
print "cat = " + str(cat)
print "dog = " + str(dog)
print "frog = " + str(frog)

suitcase = ["sunglasses", "hat", "passport", "laptop", "suit", "shoes"]
first = suitcase[:2]# the first two items
middle = suitcase[2:4]# third and fourth items
last = suitcase[4:]# the last two items

print "suitcase = " + str(suitcase)
print "Fist two items = " + str(first)
print "Middle items = "+ str(middle)
print "Last items = "+ str(last)

animals2 = ["aardvark", "badger", "duck", "emu", "fennec fox"]
print str(animals2) + """, now inserting cobra where animals2.index("duck") is."""
duck_index = animals2.index("duck")# `index()` to find "duck"
animals2.insert(duck_index, "cobra")
print animals2 # Observe what prints after the insert operation
