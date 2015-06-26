#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <string.h>
#include <strings.h>
#include <syslog.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/stat.h>
#include <signal.h>
#include <pthread.h>

#include "ivr.h"

#define MAX_FIELDS	100

/* break down the individual fields of the hpi command line */
char **getFields(char *str, char separator)
{
    char    **Fields ;
    char    *strfield ;
    char    *field, *ptr ;
    int     breakout ;
    int     i ;

    /* remove the trailing carriage return */

    ptr = strrchr (str, '\n') ;
    if (ptr)
        *ptr = '\0' ;

    Fields = (char **) calloc (1, MAX_FIELDS * sizeof (char *)) ;
    if (!Fields)
        return NULL;

    for (strfield=str, breakout=0, i=0; i<MAX_FIELDS; i++)
    {
        ptr = strchr (strfield, separator) ;
        if (!ptr)
            breakout = 1 ;
        else
            *ptr = '\0' ;

        field = (char *) calloc (1, (strlen (strfield) + 1) * sizeof (char)) ;
        if (!field)
            return NULL;
        
        strncpy (field, strfield, strlen (strfield)) ;
        Fields[i] = field ;

        if (breakout)
            break ;

        ptr++ ;
		strfield = ptr ;
    }

    return (Fields) ;
}

void freeFields (char **fields)
{
    int i ;

    for (i=0; fields[i]; i++)
    {
        free (fields[i]) ;
    }
    free (fields) ;
}

char    *getField (char **fields, char *fieldsel)
{
    int i, len ;

    if (!fields || !fieldsel)
        return (NULL) ;

    len = strlen(fieldsel) ;
    for (i=0; fields[i]; i++)
    {
        if (!strncasecmp (fields[i], fieldsel, len))
            return (&fields[i][len+1]) ;
    }
    return (NULL) ;
}

int pthread_sleep(unsigned winks)
{
    struct timeval  quantum;

    quantum.tv_usec = 0;
    quantum.tv_sec  = winks;

    return select(0, NULL, NULL, NULL, &quantum);
}

/* How we handle errors in SoftOCL - need the logger handle somehow */
void ErrorHandler(OCLSchema ocl)
{
	IVRThreader	*this ;
	char		buffer[1024] ;

	this = (IVRThreader *) ocl->appData ;

	if (this)
	{
    	sprintf(buffer, "SoftOCL[%s]-%d %s", ocl->name, ocl->errnum, ocl->errstr);
		writeLogger (this->logger, "debug", "desc", LOG_STRING, buffer, NULL) ;
	}

}

/* How we handle debuging information from SoftOCL */
void DebugHandler(OCLSchema ocl, unsigned depth)
{
	IVRThreader	*this ;
	char		buffer[1024] ;

	this = (IVRThreader *) ocl->appData ;

	if (this)
	{
    	sprintf(buffer, "%s", ocl->tmpstr);
		writeLogger (this->logger, "debug", "desc", LOG_STRING, buffer, NULL) ;
	}
}

void OCLCommsTrace(OCLComms link, char func[], int line,
        OCLTraceLevel level, char format[], ...)
{
       printf(format);
}

void	IVRdeInit (IVRThreader **Threads, IVRThreader *this)
{
	if (!this || !Threads)
		return ;

	/* free up all the OCL resources */

	writeLogger (this->logger, "debug", "desc", LOG_STRING, "Inside ivrCleanup", NULL) ;

	freeOCLData(&this->data) ;
	freeOCLRequest (&this->request) ;
	freeOCLData (&this->response) ;
	closeOCL (this->link) ;
	deinitOCLComms (&this->link) ;
	deinitSoftOCL (&this->ocl) ;

	/* free up memopry allocated for this thread */
	if (this->mbox) 
		free (this->mbox) ;
	if (this->ocnum) 
		free (this->mbox) ;
	if (this->passwd) 
		free (this->passwd) ;
	if (this->address) 
		free (this->address) ;

	/* tidy up the linked list */

	if (this->prev)
		this->prev->next = this->next ;

	if (this->next)
		this->next->prev = this->prev ;

	if (this == *Threads)
		*Threads = NULL ;

	/* and free up the thread itself */
	free (this) ;

	
}

static int  loginOCLOutcall( OCLSchema ocl, OCLComms link, unsigned timeout, 
											char *mbox, char *ocnum, int appid, char pswd[])
{
    int             ret = -1;
    int             error = 0;
    uint8           usage  = 0x02;
    OCLRequest      ready  = NULL;
    OCLData         d_usage= NULL;
    OCLData         d_mbox= NULL;
    OCLData         d_oc_num= NULL;
    OCLData         d_resp = NULL;

    /* OCL Signon to device */
    ret = signonOCL(ocl, link, appid, pswd);
    if(!ret)
        return -1;

	ret = -1 ;

    d_usage = setOCLData(ocl, NULL, "port_usage", &usage, sizeof(usage), NULL);
    if(d_usage == NULL)
        goto CleanUp;

    d_mbox = setOCLData(ocl, NULL, "mbox", mbox, strlen(mbox), NULL);
    if(d_usage == NULL)
        goto CleanUp;

    d_oc_num = setOCLData(ocl, NULL, "oc_num", ocnum, strlen(ocnum), NULL);
    if(d_usage == NULL)
        goto CleanUp;

    ready = makeOCLRequest(ocl, "Ready_Outcall", d_usage, d_mbox, d_oc_num, NULL);
    if(ready == NULL)
        goto CleanUp;

    d_resp = recvOCLResponse(ocl, link, ready, timeout);
    if(d_resp == NULL)
        goto CleanUp;
    else if((error = errorCheckOCL(ocl, d_resp)) != 0)
        goto CleanUp;

    freeOCLData(&d_resp);

    ret = 0;

CleanUp:
    if(d_resp)      freeOCLData(&d_resp);
    if(d_usage)     freeOCLData(&d_usage);
    if(d_mbox)      freeOCLData(&d_mbox);
    if(d_oc_num)    freeOCLData(&d_oc_num);
    if(ready)       freeOCLRequest(&ready);

    return ret;
}


static void	*ivrThread(IVRThreader *this)
{
	uint8		phrase_format ;
	uint16		phrase_id ;
	uint16		retcode ;
	char		buffer[1024] ;

	this->logger = initLoggerThread (this->sysLogger, this->tid) ;

	sprintf (buffer, "Thread %d starting", this->tid) ;
	writeLogger (this->logger, "debug", "desc", LOG_STRING, buffer,
										 "function", LOG_STRING, "IVRThread", 
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;

    /* init the softOCL and make connection to VP */

    this->ocl = initSoftOCL("/home/andix/misc/sharedObjects/ivr/VIS3.0", ErrorHandler, NULL, FALSE, DebugHandler, NULL) ;
    if (!this->ocl)
    {
		writeLogger (this->logger, "error", "desc", LOG_STRING, "ERROR: Could not init OCL Schema",
										 	"function", LOG_STRING, "IVRThread", 
										 	"module", LOG_STRING, "IVR_Outcall", NULL) ;
		this->result = -1 ;
		pthread_exit((void *)0) ;
    }

	/* attach the threader data to the OCL schema */
	this->ocl->appData = (void *) this ;

    this->link = initOCLComms (TCP_NETWORK, NO_TRACE, stdout, OCLCommsTrace) ;
    if (!link)
    {
		writeLogger (this->logger, "error", "desc", LOG_STRING, "ERROR: Could not init OCL Comms", 
										 	"function", LOG_STRING, "IVRThread", 
										 	"module", LOG_STRING, "IVR_Outcall", NULL) ;
		this->result = -1 ;
		pthread_exit((void *)0) ;
    }

    this->result = openOCL(this->link, this->address) ;
    if (this->result < 0)
    {
		writeLogger (this->logger, "error", "desc", LOG_STRING, "ERROR: Could not open OCL",
										 	"function", LOG_STRING, "IVRThread", 
										 	"module", LOG_STRING, "IVR_Outcall", NULL) ;
		pthread_exit((void *)0) ;
    }

	this->result = loginOCLOutcall (this->ocl, this->link, this->timeout, this->mbox, 
													this->ocnum, this->appid, this->passwd) ;
	if (this->result < 0)
    {
		writeLogger (this->logger, "error", "desc", LOG_STRING, "ERROR: Could not open an IVR connection",
										 	"function", LOG_STRING, "IVRThread", 
										 	"module", LOG_STRING, "IVR_Outcall", NULL) ;
		pthread_exit((void *)0) ;
    }

	/* OK - all we are going to do is play a phrase and disconnect */

	phrase_format = 2 ; /* system phrase */
	phrase_id = 282 ; /* system phrase 282 */
    this->data = setOCLData (this->ocl, NULL, "phrase.format", &phrase_format, sizeof(phrase_format), 
											  "phrase.id", &phrase_id, sizeof(phrase_id),
												NULL) ;
    if (!this->data)
    {
		writeLogger (this->logger, "error", "desc", LOG_STRING, "ERROR: setting OCL data",
										 	"function", LOG_STRING, "IVRThread", 
										 	"module", LOG_STRING, "IVR_Outcall", NULL) ;
		this->result = -1 ;
		pthread_exit((void *)0) ;
    }

    this->request = makeOCLRequest (this->ocl, "Phrase_Play", this->data, NULL) ;
    if (!this->request)
    {
		writeLogger (this->logger, "error", "desc", LOG_STRING, "ERROR: making Phrase_Play request", 
										 	"function", LOG_STRING, "IVRThread", 
										 	"module", LOG_STRING, "IVR_Outcall", NULL) ;
		this->result = -1 ;
		pthread_exit((void *)0) ;
    }
    this->response = recvOCLResponse (this->ocl, this->link, this->request, this->timeout) ;
    if (!this->response)
    {
		writeLogger (this->logger, "error", "desc", LOG_STRING, "ERROR: receiving Phrase_Play response",
										 	"function", LOG_STRING, "IVRThread", 
										 	"module", LOG_STRING, "IVR_Outcall", NULL) ;
		this->result = -1 ;
		pthread_exit((void *)0) ;
    }
    getOCLData (this->ocl, this->response, "rc", &retcode, sizeof(retcode), NULL) ;
    if (retcode)
    {
		writeLogger (this->logger, "error", "desc", LOG_STRING, "ERROR: in Phrase_Play response",
											"rc", LOG_INT, (int) retcode,
										 	"function", LOG_STRING, "IVRThread", 
										 	"module", LOG_STRING, "IVR_Outcall", NULL) ;
		this->result = (int) retcode ;
		pthread_exit((void *)0) ;
    }
	
	freeOCLData(&this->data) ;
	freeOCLRequest (&this->request) ;
	freeOCLData (&this->response) ;

	/* now just need to disconnect */

    this->request = makeOCLRequest (this->ocl, "Port_Disconnect", NULL) ;
    if (!this->request)
    {
		writeLogger (this->logger, "error", "desc", LOG_STRING, "ERROR: making Port_Disconnect request",
										 	"function", LOG_STRING, "IVRThread", 
										 	"module", LOG_STRING, "IVR_Outcall", NULL) ;
		this->result = -1 ;
		pthread_exit((void *)0) ;
    }
    this->response = recvOCLResponse (this->ocl, this->link, this->request, this->timeout) ;
    if (!this->response)
    {
		writeLogger (this->logger, "error", "desc", LOG_STRING, "ERROR: receiving Port_Disconnect response", 
										 	"function", LOG_STRING, "IVRThread", 
										 	"module", LOG_STRING, "IVR_Outcall", NULL) ;
		this->result = -1 ;
		pthread_exit((void *)0) ;
    }
    getOCLData (this->ocl, this->response, "rc", &retcode, sizeof(retcode), NULL) ;
    if (retcode)
    {
		writeLogger (this->logger, "error", "desc", LOG_STRING, "ERROR: in Port_Disconnect response",
											"rc", LOG_INT, (int) retcode,
										 	"function", LOG_STRING, "IVRThread", 
										 	"module", LOG_STRING, "IVR_Outcall", NULL) ;
		this->result = (int) retcode ;
		pthread_exit((void *)0) ;
    }

	/* that's your lot - notice that we don't free the threader structure.
		This is the job of the application calling this shared object */

	this->result = 0; /* success */
	pthread_exit((void *)0) ;
}

int IVRInit(IVRThreader **Threads, Logger sysLogger, char *hpi_request)
{
    IVRThreader     *thread ;
	char			**fields ;
	char			*address = NULL ;
	char			*appid = NULL ;
	char			*ocnum = NULL  ;
	char			*mbox = NULL ;
	char			*passwd = NULL ;

	if (!Threads)
	{
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "No Threader linked list passed", 
										 "function", LOG_STRING, "IVRInit", 
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
		return (-1) ;
	}

	/* might be an idea to have something to do */
	if (!hpi_request)
	{
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "No hpi request passed", 
										 "function", LOG_STRING, "IVRInit", 
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
		return (-1) ;
	}

	/* create a thread to handle the request */

    thread = (IVRThreader *) calloc (1, sizeof (IVRThreader)) ;
    if (!thread)
	{
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "Could not allocate threader structure", 
										 "function", LOG_STRING, "IVRInit", 
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
        return (-1) ;
	}

    thread->hpi_request = (char *) calloc (1, strlen(hpi_request) + 1) ;
    if (!thread->hpi_request)
	{
		free (thread) ;
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "Memory Allocation Failure ", 
										 "function", LOG_STRING, "IVRInit",
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
        return (-1) ;
	}
	strncpy (thread->hpi_request, hpi_request, strlen(hpi_request)) ;

	fields = getFields (thread->hpi_request, ':') ;
	if (!fields)
	{
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "Could not read hpi request fields", 
										 "function", LOG_STRING, "IVRInit",
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
		return (-1) ;
	}

	address = getField (fields, "address") ;
	if (!address)
	{
		free (thread) ;
		free (fields) ;
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "Could not find address field", 
										 "function", LOG_STRING, "IVRInit",
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
		return (-1) ;
	}

    thread->address = (char *) calloc (1, strlen(address) + 1) ;
    if (!thread->address)
	{
		free (thread) ;
		free (fields) ;
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "Memory Allocation Failure ", 
										 "function", LOG_STRING, "IVRInit",
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
        return (-1) ;
	}
	strncpy (thread->address, address, strlen(address)) ;

	passwd = getField (fields, "passwd") ;
	if (!passwd)
	{
		free (thread) ;
		free (fields) ;
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "Could not find passwd field", 
										 "function", LOG_STRING, "IVRInit",
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
		return (-1) ;
	}

    thread->passwd = (char *) calloc (1, strlen(passwd) + 1) ;
    if (!thread->passwd)
	{
		free (thread) ;
		free (fields) ;
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "Memory Allocation Failure ", 
										 "function", LOG_STRING, "IVRInit",
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
        return (-1) ;
	}
	strncpy (thread->passwd, passwd, strlen(passwd)) ;

	ocnum = getField (fields, "ocnum") ;
	if (!ocnum)
	{
		free (thread) ;
		free (fields) ;
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "Could not find ocnum field", 
										 "function", LOG_STRING, "IVRInit",
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
		return (-1) ;
	}

    thread->ocnum = (char *) calloc (1, strlen(ocnum) + 1) ;
    if (!thread->ocnum)
	{
		free (thread) ;
		free (fields) ;
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "Memory Allocation Failure ", 
										 "function", LOG_STRING, "IVRInit",
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
        return (-1) ;
	}
	strncpy (thread->ocnum, ocnum, strlen(ocnum)) ;

	mbox = getField (fields, "mbox") ;
	if (!mbox)
	{
		free (thread) ;
		free (fields) ;
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "Could not find mbox field", 
										 "function", LOG_STRING, "IVRInit",
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
		return (-1) ;
	}

    thread->mbox = (char *) calloc (1, strlen(mbox) + 1) ;
    if (!thread->mbox)
	{
		free (thread) ;
		free (fields) ;
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "Memory Allocation Failure ", 
										 "function", LOG_STRING, "IVRInit",
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
        return (-1) ;
	}
	strncpy (thread->mbox, mbox, strlen(mbox)) ;

	thread->main = ivrThread ;
	thread->sysLogger = sysLogger ;
	thread->timeout = 300 ; /* what is a sensible value for timeout ? */


	appid = getField (fields, "appid") ;
	if (!appid)
	{
		free (thread) ;
		free (fields) ;
		writeLogger (sysLogger, "error", "desc", LOG_STRING, "Could not find appid field", 
										 "function", LOG_STRING, "IVRInit",
										 "module", LOG_STRING, "IVR_Outcall", NULL) ;
		return (-1) ;
	}

	thread->appid = atoi(appid) ;

	/* slot the new thread into the linked list provided */

    thread->next = *Threads ;
    *Threads = thread ;
    if (thread->next)
        thread->next->prev = thread ;

	/* free up the allocated memory */
	free (fields) ;

	/* cue thread */

    pthread_attr_init(&thread->tattr);
    pthread_create(&thread->tid, &thread->tattr, (void*)thread->main, thread);

	return (0) ;
}
