#!/usr/bin/python

import sys
import httplib2

def linebreak():
  retval="\n"
  for i in range(1,indent):
    retval+="  "
  return retval
  
# Dirty hack to "source" config files
def execfile(file, globals=globals(), locals=locals()):
  try:
    with open(file, "r") as fh:
      exec(fh.read()+"\n", globals, locals)
  except:
#    print("config file not found")
    pass
        
def introspectHttp(host, port):
  h = httplib2.Http()  
  res, content = h.request("http://" + host + ":" + str(port) + "/jsonrpc", "POST", "{\"id\":0,\"jsonrpc\":\"2.0\", \"method\":\"JSONRPC.Introspect\"}")#, headers)
  #print(res)
  #print(content)
  retval=""
  retval+=content.decode("utf-8")
  return retval
  
def printFile(filename, output):
  f = open(filename, "w")
  f.write(output)
  

host="localhost"
port=8080
outputfile="xbmc/api/xbmcapi.json"

config_file = '.fetchapi_config.py'
execfile(config_file)

nextishost = 0
nextisport = 0
nextisof = 0
for arg in sys.argv:
#  print("command line arg", arg)
  if(nextishost == 1):
    host = arg
    nextishost = 0
  if(nextisport == 1):
    port = arg
    nextisport = 0
  if(nextisof == 1):
    outputfile = arg
    nextisof = 0
  if(arg == "--help"):
    print("usage:", sys.argv[0], "[-h <ip>] [-p <port>] [-o <outputfile>]")
    exit(0)
  if(arg == "-h"):
    nextishost = 1
  if(arg == "-p"):
    nextisport = 1
  if(arg == "-o"):
    nextisof = 1
    


print("connecting to host:", host)

bracecount=0
outputjson=""
lastchar=""
indent=0
methodstring=""
methodlevel=-1

data=introspectHttp(host, port)
if not data=="":
  for char in data:
    prefix=""
    suffix=""
    
#    if(char=="\""):
#      if(lastchar==","):
#        prefix+=linebreak();
        
    if(char=="{") or (char=="["):
      bracecount+=1
      indent+=1
      suffix+=linebreak()

    if(char=="}") or (char=="]"):
      bracecount-=1
      indent-=1
      prefix+=linebreak()
    
    if(char==","):
#      if(lastchar=="}"):
      suffix+=linebreak()
      if bracecount==methodlevel+1:
        suffix+=linebreak()
    
    # reassemble the string
    outputjson+=prefix
    outputjson+=char
    outputjson+=suffix

        # do some other thecks
    methodstring+=char
    tmp="methods"
    if(methodstring==tmp):
      methodlevel=bracecount
      methodstring=""
#      print("methodlevel is", methodlevel)
      
    if not tmp.startswith(methodstring):
      methodstring=""

    
#    print("got brace. total:", bracecount)
    lastchar=char
    
    
printFile(outputfile, outputjson)

