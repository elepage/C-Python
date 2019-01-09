import emb
import struct
import binascii
import ctypes
from ctypes import *

print "Number of arguments", emb.numargs()
print "Test2", emb.test2()
print "Test3", emb.test3("Hello World",2,6)

values = (1, 'ab', 2.7)
s = struct.Struct('I 2s f')
packed_data = s.pack(*values)

print 'Original values:', values
print 'Format string  :', s.format
print 'Uses           :', s.size, 'bytes'
print 'Packed Value   :', binascii.hexlify(packed_data)

def multiply(a,b):
    print "Will compute", a, "times", b
    c = 0
    for i in range(0, a):
        c = c + b
    return c

def multiplyWithUserData(a,b,pxUserData):
    print "multiplyWithUserData", emb.getArrayElement(pxUserData,2)
    print "Will compute", a, "times", b
    c = 0
    for i in range(0, a):
        c = c + b
    return c

def multiplyWithUserData(a,b,pxUserData):
    INTP = ctypes.POINTER(ctypes.c_int32)
    piInput = emb.getIntArrayPointer(pxUserData,1)
    piOutput = emb.getIntArrayPointer(pxUserData,0)
    iInputSize = emb.getIntArraySize(pxUserData,1)
    iOutputSize = emb.getIntArraySize(pxUserData,0)
    print "piInput",piInput,"piOutput",piOutput
    print "multiplyWithUserData", emb.getArrayElement(pxUserData,2)
    for i in range(0,iInputSize):
        pi = ctypes.cast(piInput+4*i,INTP)
        po = ctypes.cast(piOutput+4*i,INTP)
        po.contents.value = pi.contents.value*3
	print i,"Input:(",piInput+4*i,",",pi.contents.value,"),Output:(", piOutput+4*i,",", po.contents.value,")"
    print "Will compute", a, "times", b
    c = 0
    for i in range(0, a):
        c = c + b
    return c
