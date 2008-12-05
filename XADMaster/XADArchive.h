#import <Foundation/Foundation.h>

#import "CSHandle.h"
#import "XADException.h"

typedef int XADAction;

#define XADAbortAction 0
#define XADRetryAction 1
#define XADSkipAction 2
#define XADOverwriteAction 3
#define XADRenameAction 4



@class UniversalDetector;

@interface XADArchive:NSObject
{
	CSHandle *inputhandle;
	NSString *filename;
	NSArray *volumes;

	BOOL encrypted,solid;

	id delegate;
	NSString *password;
	NSTimeInterval update_interval;

	XADError lasterror;

//	NSMutableDictionary *dittoforks;
//	NSMutableArray *writeperms;

/*	int currentry;
	xadSize extractsize,totalsize;
	NSString *immediatedestination;
	BOOL immediatefailed;*/
}

+(XADArchive *)archiveForFile:(NSString *)filename;
+(XADArchive *)recursiveArchiveForFile:(NSString *)filename;

+(NSArray *)volumesForFile:(NSString *)filename;



// Constructors implemented directly by XADArchive through -initWithFile:handle:

-(id)init;
-(id)initWithFile:(NSString *)file;
-(id)initWithFile:(NSString *)file error:(XADError *)error;
-(id)initWithFile:(NSString *)file delegate:(id)del error:(XADError *)error;
-(id)initWithData:(NSData *)data;
-(id)initWithData:(NSData *)data error:(XADError *)error;
-(id)initWithArchive:(XADArchive *)archive entry:(int)n;
-(id)initWithArchive:(XADArchive *)archive entry:(int)n error:(XADError *)error;
-(id)initWithArchive:(XADArchive *)otherarchive entry:(int)n
     immediateExtractionTo:(NSString *)destination error:(XADError *)error;
-(void)dealloc;



// XADArchive methods

-(NSString *)filename;
-(NSArray *)allFilenames;

-(BOOL)isEncrypted;
-(BOOL)isSolid;
-(BOOL)isCorrupted;

-(int)numberOfEntries;
-(NSDictionary *)propertiesForEntry:(int)n;

-(BOOL)immediateExtractionFailed;

-(NSString *)commonTopDirectory;

-(void)setDelegate:(id)delegate;
-(id)delegate;

-(NSString *)password;
-(void)setPassword:(NSString *)newpassword;

-(NSStringEncoding)nameEncoding;
-(void)setNameEncoding:(NSStringEncoding)encoding;

-(XADError)lastError;
-(void)clearLastError;
-(NSString *)describeLastError;
-(NSString *)describeError:(XADError)error;

-(NSString *)description;



// Data access methods implemented by XADArchive through the subclass methods

-(NSString *)nameOfEntry:(int)n;
-(BOOL)entryHasSize:(int)n;
-(int)sizeOfEntry:(int)n;
-(BOOL)entryIsDirectory:(int)n;
-(BOOL)entryIsLink:(int)n;
-(BOOL)entryIsEncrypted:(int)n;
-(BOOL)entryIsArchive:(int)n;
-(BOOL)entryHasResourceFork:(int)n;
-(NSDictionary *)attributesOfEntry:(int)n;
-(NSDictionary *)attributesOfEntry:(int)n withResourceFork:(BOOL)resfork;
-(CSHandle *)handleForEntry:(int)n;
-(CSHandle *)resourceHandleForEntry:(int)n;
-(NSData *)contentsOfEntry:(int)n;
-(NSData *)resourceContentsOfEntry:(int)n;

-(BOOL)extractTo:(NSString *)destination;
-(BOOL)extractTo:(NSString *)destination subArchives:(BOOL)sub;
-(BOOL)extractEntries:(NSIndexSet *)entries to:(NSString *)destination;
-(BOOL)extractEntries:(NSIndexSet *)entries to:(NSString *)destination subArchives:(BOOL)sub;
-(BOOL)extractEntry:(int)n to:(NSString *)destination;
-(BOOL)extractEntry:(int)n to:(NSString *)destination overrideWritePermissions:(BOOL)override;
-(BOOL)extractArchiveEntry:(int)n to:(NSString *)destination;
-(void)fixWritePermissions;

-(BOOL)_extractEntry:(int)n as:(NSString *)destfile;
-(BOOL)_extractFileEntry:(int)n as:(NSString *)destfile;
-(BOOL)_extractDirectoryEntry:(int)n as:(NSString *)destfile;
-(BOOL)_extractLinkEntry:(int)n as:(NSString *)destfile;
-(xadERROR)_extractFileInfo:(struct xadFileInfo *)info tags:(xadTAGPTR)tags reportProgress:(BOOL)report;
-(BOOL)_ensureDirectoryExists:(NSString *)directory;
-(BOOL)_changeAllAttributes:(NSDictionary *)attrs atPath:(NSString *)path overrideWritePermissions:(BOOL)override;

@end



@interface NSObject (XADArchiveDelegate)

-(NSStringEncoding)archive:(XADArchive *)archive encodingForName:(const char *)bytes guess:(NSStringEncoding)guess confidence:(float)confidence;
-(XADAction)archive:(XADArchive *)archive nameDecodingDidFailForEntry:(int)n bytes:(const char *)bytes;

-(BOOL)archiveExtractionShouldStop:(XADArchive *)archive;
-(BOOL)archive:(XADArchive *)archive shouldCreateDirectory:(NSString *)directory;
-(XADAction)archive:(XADArchive *)archive entry:(int)n collidesWithFile:(NSString *)file newFilename:(NSString **)newname;
-(XADAction)archive:(XADArchive *)archive entry:(int)n collidesWithDirectory:(NSString *)file newFilename:(NSString **)newname;
-(XADAction)archive:(XADArchive *)archive creatingDirectoryDidFailForEntry:(int)n;

-(void)archive:(XADArchive *)archive extractionOfEntryWillStart:(int)n;
-(void)archive:(XADArchive *)archive extractionProgressForEntry:(int)n bytes:(xadSize)bytes of:(xadSize)total;
-(void)archive:(XADArchive *)archive extractionOfEntryDidSucceed:(int)n;
-(XADAction)archive:(XADArchive *)archive extractionOfEntryDidFail:(int)n error:(XADError)error;
-(XADAction)archive:(XADArchive *)archive extractionOfResourceForkForEntryDidFail:(int)n error:(XADError)error;

-(void)archive:(XADArchive *)archive extractionProgressBytes:(xadSize)bytes of:(xadSize)total;
-(void)archive:(XADArchive *)archive extractionProgressFiles:(int)files of:(int)total;

@end


#ifndef XAD_NO_DEPRECATED

#define XADERR_NO XADNoError
#define XADNoError
#define XADUnknownError          0x0001 /* unknown error */
#define XADInputError            0x0002 /* input data buffers border exceeded */
#define XADOutputError           0x0003 /* output data buffers border exceeded */
#define XADBadParametersError    0x0004 /* function called with illegal parameters */
#define XADOutOfMemoryError      0x0005 /* not enough memory available */
#define XADIllegalDataError      0x0006 /* data is corrupted */
#define XADNotSupportedError     0x0007 /* command is not supported */
#define XADResourceError         0x0008 /* required resource missing */
#define XADDecrunchError         0x0009 /* error on decrunching */
#define XADFiletypeError         0x000A /* unknown file type */
#define XADOpenFileError         0x000B /* opening file failed */
#define XADSkipError             0x000C /* file, disk has been skipped */
#define XADBreakError            0x000D /* user break in progress hook */
#define XADFileExistsError       0x000E /* file already exists */
#define XADPasswordError         0x000F /* missing or wrong password */
#define XADMakeDirectoryError    0x0010 /* could not create directory */
#define XADChecksumError         0x0011 /* wrong checksum */
#define XADVerifyError           0x0012 /* verify failed (disk hook) */
#define XADGeometryError         0x0013 /* wrong drive geometry */
#define XADDataFormatError       0x0014 /* unknown data format */
#define XADEmptyError            0x0015 /* source contains no files */
#define XADFileSystemError       0x0016 /* unknown filesystem */
#define XADFileDirectoryError    0x0017 /* name of file exists as directory */
#define XADShortBufferError      0x0018 /* buffer was too short */
#define XADEncodingError         0x0019 /* text encoding was defective */

#define XADAbort XADAbortAction
#define XADRetry XADRetryAction
#define XADSkip XADSkipAction
#define XADOverwrite XADOverwriteAction
#define XADRename XADRenameAction

#endif