/*
 *  url.h - URL Processing (parsing & retrieval)
 *
 *  Darren Hardy, hardy@cs.colorado.edu, April 1994
 *
 *  $Id: url.h,v 1.2 2006/02/03 16:59:14 golda Exp $
 *
 *  ----------------------------------------------------------------------
 *  Copyright (c) 1994, 1995.  All rights reserved.
 *  
 *          Mic Bowman of Transarc Corporation.
 *          Peter Danzig of the University of Southern California.
 *          Darren R. Hardy of the University of Colorado at Boulder.
 *          Udi Manber of the University of Arizona.
 *          Michael F. Schwartz of the University of Colorado at Boulder. 
 *  
 *  This copyright notice applies to all code in Harvest other than
 *  subsystems developed elsewhere, which contain other copyright notices
 *  in their source text.
 *  
 *  The Harvest software was developed by the Internet Research Task
 *  Force Research Group on Resource Discovery (IRTF-RD).  The Harvest
 *  software may be used for academic, research, government, and internal
 *  business purposes without charge.  If you wish to sell or distribute
 *  the Harvest software to commercial clients or partners, you must
 *  license the software.  See
 *  http://harvest.cs.colorado.edu/harvest/copyright,licensing.html#licensing.
 *  
 *  The Harvest software is provided ``as is'', without express or
 *  implied warranty, and with no support nor obligation to assist in its
 *  use, correction, modification or enhancement.  We assume no liability
 *  with respect to the infringement of copyrights, trade secrets, or any
 *  patents, and are not responsible for consequential damages.  Proper
 *  use of the Harvest software is entirely the responsibility of the user.
 *  
 *  For those who are using Harvest for non-commercial purposes, you may
 *  make derivative works, subject to the following constraints:
 *  
 *  - You must include the above copyright notice and these accompanying 
 *    paragraphs in all forms of derivative works, and any documentation 
 *    and other materials related to such distribution and use acknowledge 
 *    that the software was developed at the above institutions.
 *  
 *  - You must notify IRTF-RD regarding your distribution of the 
 *    derivative work.
 *  
 *  - You must clearly notify users that your are distributing a modified 
 *    version and not the original Harvest software.
 *  
 *  - Any derivative product is also subject to the restrictions of the 
 *    copyright, including distribution and use limitations.
 */
#ifndef _URL_H_
#define _URL_H_

#include "config.h"
#include <time.h>

/*
 *  The supported URLs look like:
 *
 *      file://host/pathname
 *      gopher://host[:port][/TypeDigitGopherRequest]
 *      http://host[:port][/[pathname][#name][?search]]
 *      ftp://[user[:password]@]host[:port][/pathname]
 *
 *  where host is either a fully qualified hostname, an IP number, or
 *  a relative hostname.  
 *
 *  For http, any '#name' or '?search' directives are ignored.
 *  For ftp, any user, password, or port directives are unsupported.
 */

struct url {
	char *url;		/* Complete, normalized URL */
	int type;		/* file, ftp, http, gopher, etc. */
	char *raw_pathname;	/* pathname portion of the URL, w/ escapes */
	char *pathname;		/* pathname portion of the URL, w/o escapes */
	char *host;		/* fully qualified hostname */
	int port;		/* TCP/IP port */


/* Information for FTP processing */
	char *user;		/* Login name for ftp */
	char *password;		/* password for ftp */

/* Information for Gopher processing */
	int gophertype;		/* Numeric type for gopher request */

/* Information for HTTP processing */
	char *http_version;	/* HTTP/1.0 Version */
	int   http_status_code;	/* HTTP/1.0 Status Code */
	char *http_reason_line;	/* HTTP/1.0 Reason Line */
	char *http_mime_hdr;	/* HTTP/1.0 MIME Response Header */

/* Information for local copy processing */
	char *filename;		/* local filename */
	FILE *fp;		/* ptr to local filename */
	time_t lmt;		/* Last-Modification-Time */

#ifdef USE_MD5
	char *md5;		/* MD5 value of URL */
#endif
};
typedef struct url URL;

enum url_types {		/* Constants for URL types */ 
	URL_UNKNOWN,
	URL_FILE,
	URL_FTP,
	URL_GOPHER,
	URL_HTTP,
	URL_NEWS,
	URL_NOP,
	URL_TELNET,
	URL_WAIS
};

#ifndef _PARAMS
#define _PARAMS(ARGS) ARGS
#endif /* _PARAMS */   

URL *url_open _PARAMS((char *));
int url_read _PARAMS((char *, int, int, URL *));
int url_retrieve _PARAMS((URL *));
void url_close _PARAMS((URL *));

void init_url _PARAMS(());
void finish_url _PARAMS(());

void url_purge _PARAMS(());

URL *dup_url _PARAMS((URL *));
void print_url _PARAMS((URL *));

int http_get _PARAMS((URL *));
int ftp_get _PARAMS((URL *));
int gopher_get _PARAMS((URL *));


#ifdef USE_LOCAL_CACHE
void init_cache _PARAMS(());
char *lookup_cache _PARAMS((char *));
time_t lmt_cache _PARAMS((char *));
void add_cache _PARAMS((char *, char *));
void finish_cache _PARAMS(());
void expire_cache _PARAMS(());
#endif 

#ifdef USE_CCACHE
void url_initCache _PARAMS((int, long));
void url_shutdowncache _PARAMS(());
#endif

#endif /* _URL_H_ */

