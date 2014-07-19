from biplist import *
from datetime import datetime

filename = "config.plist";
filename_new = "config_new.plist";
try:
    plist = readPlist(filename)
    npcDict = plist["npc"]
    for key in npcDict:
    	tmpNpc = npcDict[key]
    	if key.startswith("100"):
    		tmpNpc["record"] = 10
    	elif key.startswith("101"):
    		tmpNpc["record"] = 50
    	elif key.startswith("102"):
    		tmpNpc["record"] = 100

    writePlist(plist, filename_new,False)
except (InvalidPlistException, NotBinaryPlistException), e:
    print "Something bad happened:", e