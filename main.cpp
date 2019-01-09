#include <Python.h>

static int numargs=0;
static int test2=0;
static int* pitest3=NULL;

#define ARRAY_DIM 5

static PyObject* emb_numargs(PyObject *self, PyObject *args){
    if(!PyArg_ParseTuple(args, ":numargs"))
        return NULL;
    return Py_BuildValue("i", numargs);
}

static PyObject* emb_test2(PyObject *self, PyObject *args){
    if(!PyArg_ParseTuple(args, ":test2"))
        return NULL;
    return Py_BuildValue("i", test2);
}

static PyObject* emb_test3(PyObject *self, PyObject *args){
    PyObject *pyStr = NULL;
    int arg1, arg2;
    arg2 = 8;
    if (! PyArg_ParseTuple(args, "Si|i:test3", &pyStr, &arg1, &arg2)) 
        return NULL;
	if(arg1>=ARRAY_DIM)
		arg1 =ARRAY_DIM-1;
	printf("This is C %s;%d,%d\n",PyString_AsString(pyStr),arg1,arg2);
    assert(! PyErr_Occurred());
    return Py_BuildValue("i", pitest3[arg1]);
}

typedef struct{
	int *piIntInputArray;
	int iIntInputArraySize;
	int *piIntOutputArray;
	int iIntOutputArraySize;
}UserData;

static PyObject* emb_getArrayElement(PyObject *self, PyObject *args){
    int arg1;
	UserData* pxUserData=NULL;
    if (! PyArg_ParseTuple(args, "li:test3", (long*)&pxUserData, &arg1)) 
        return NULL;
	if(arg1>=pxUserData->iIntInputArraySize)
		arg1 =pxUserData->iIntInputArraySize-1;
	printf("This is C %ld;%d\n",pxUserData,arg1);
    assert(! PyErr_Occurred());
	return Py_BuildValue("i", pxUserData?pxUserData->piIntInputArray[arg1]:0);
}

static PyObject* emb_getIntArrayPointer(PyObject *self, PyObject *args){
	UserData* pxUserData=NULL;
	int iInput =0;
    if (! PyArg_ParseTuple(args, "li:test3", (long*)&pxUserData,&iInput)) 
        return NULL;
	printf("(This is C %ld|%d)",pxUserData,iInput);
    assert(! PyErr_Occurred());
	return Py_BuildValue("l", pxUserData?(iInput?pxUserData->piIntInputArray:pxUserData->piIntOutputArray):NULL);
}

static PyObject* emb_getIntArraySize(PyObject *self, PyObject *args){
	UserData* pxUserData=NULL;
	int iInput =0;
    if (! PyArg_ParseTuple(args, "li:test3", (long*)&pxUserData,&iInput)) 
        return NULL;
	printf("(This is C %ld|%d)",pxUserData,iInput);
    assert(! PyErr_Occurred());
	return Py_BuildValue("i", pxUserData?(iInput?pxUserData->iIntInputArraySize:pxUserData->iIntOutputArraySize):0);
}

static PyMethodDef EmbMethods[] = {
    {"numargs", emb_numargs, METH_VARARGS,"Return the number of arguments received by the process."},
    {"test2", emb_test2, METH_VARARGS,"Return variable test2."},
    {"test3", emb_test3, METH_VARARGS,"Return variable test3."},
    {"getArrayElement", emb_getArrayElement, METH_VARARGS,"Return getArrayElement."},
    {"getIntArrayPointer", emb_getIntArrayPointer, METH_VARARGS,"Return getArrayPointer."},
    {"getIntArraySize", emb_getIntArraySize, METH_VARARGS,"Return getArraySize."},
    {NULL, NULL, 0, NULL}
};

int main(int argc, char *argv[]){
    PyObject *pName, *pModule, *pFunc;
    PyObject *pArgs, *pValue;
	UserData *pxUserData = (UserData *)calloc(1,sizeof(UserData));
    int i;
	int iTest2 = 28;
	int aiInput[ARRAY_DIM] = {1,2,3,4,5};
	int aiOutput[ARRAY_DIM] = {1,2,3,4,5};
	test2 = iTest2; 
	pitest3 = aiInput;
	pxUserData->piIntInputArray=aiInput;
	pxUserData->iIntInputArraySize=ARRAY_DIM;
	pxUserData->piIntOutputArray=aiOutput;
	pxUserData->iIntOutputArraySize=ARRAY_DIM;

	printf("Pointers:%ld,%ld,%ld\n",pxUserData,pxUserData->piIntInputArray,pxUserData->piIntOutputArray);
	printf("sizeof:%d,%d\n",sizeof(iTest2),sizeof(aiInput));

    if (argc < 3) {
        fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
        return 1;
    }

    Py_Initialize();
	numargs = argc;
	Py_InitModule("emb", EmbMethods);
    pName = PyString_FromString(argv[1]);
    /* Error checking of pName left out */

    pModule = PyImport_Import(pName);
    Py_DECREF(pName);

    if (pModule != NULL) {
		printf("************************************************************\n");
        pFunc = PyObject_GetAttrString(pModule, argv[2]);
        /* pFunc is a new reference */

        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(argc - 3);
            for (i = 0; i < argc - 3; ++i) {
                pValue = PyInt_FromLong(atoi(argv[i + 3]));
                if (!pValue) {
                    Py_DECREF(pArgs);
                    Py_DECREF(pModule);
                    fprintf(stderr, "Cannot convert argument\n");
                    return 1;
                }
                /* pValue reference stolen here: */
                PyTuple_SetItem(pArgs, i, pValue);
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
                Py_DECREF(pValue);
            }else{
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
        }else{
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
		printf("************************************************************\n");
        pFunc = PyObject_GetAttrString(pModule, "multiplyWithUserData");
        if (pFunc && PyCallable_Check(pFunc)) {
            pArgs = PyTuple_New(3);
            pValue = PyInt_FromLong(atoi(argv[3]));
            if (pValue){
				PyTuple_SetItem(pArgs, 0, pValue);
				pValue = PyInt_FromLong(atoi(argv[4]));
				if (pValue){
					PyTuple_SetItem(pArgs, 1, pValue);
					pValue = PyLong_FromLong((long)pxUserData);
					if (pValue)
						PyTuple_SetItem(pArgs, 2, pValue);
				}
			}
            if (!pValue) {
				Py_DECREF(pArgs);
                Py_DECREF(pModule);
                fprintf(stderr, "Cannot convert argument\n");
                return 1;
            }
            pValue = PyObject_CallObject(pFunc, pArgs);
            Py_DECREF(pArgs);
            if (pValue != NULL) {
                printf("Result of call: %ld\n", PyInt_AsLong(pValue));
				for(int i=0;i<pxUserData->iIntInputArraySize;i++){
					printf("[%d]:%d|%d\n",i,pxUserData->piIntInputArray[i],pxUserData->piIntOutputArray[i]);
				}
                Py_DECREF(pValue);
            } else {
                Py_DECREF(pFunc);
                Py_DECREF(pModule);
                PyErr_Print();
                fprintf(stderr,"Call failed\n");
                return 1;
            }
		}else{
            if (PyErr_Occurred())
                PyErr_Print();
            fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
        }
        Py_XDECREF(pFunc);
        Py_DECREF(pModule);
		printf("************************************************************\n");
    }else{
        PyErr_Print();
        fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
        return 1;
    }
    Py_Finalize();
	if(pxUserData)
		free(pxUserData);
    return 0;
}
