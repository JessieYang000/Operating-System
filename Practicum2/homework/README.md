# Remote File System (RFS)

This project implements a basic **Remote File System (RFS)** using TCP sockets in C. It allows clients to perform file operations on a centralized remote server, including:
- Writing local files to the remote server
- Fetching remote files to local directories
- Removing files or directories on the remote server
- Concurrent client support with file locking for consistency

## Supported Operations

### 1. **WRITE**  
Upload a file from a local path to a remote server path.

- If the remote path doesn't exist, it will be created.
- If the local path doesn't exist, the user will be prompted again.
- If the command format is invalid, the user will be prompted again.

### 2. **GET**  
Download a file from the remote server to a local path.

- If the local path doesn't exist, it will be created.
- If the remote file doesn't exist, the user will be prompted again.
- If the command format is invalid, the user will be prompted again.

### 3. **RM**  
Delete a remote file or directory.

- If the remote path doesn't exist, the user will be prompted again.
- If the command format is invalid, the user will be prompted again.

### 4. **Multiple Clients (Concurrency)**  
The server supports multiple clients concurrently performing read/write/delete operations. File locking (`flock`) ensures safe concurrent access.


## Commands for Testing
Before testing, run `make clean && make` to compile the program. Then run `./server` and `./client` to start the server and client correspondingly.

- Write
    - valid: `rfs WRITE data/localfoo.txt folder/foo.txt`
    - invalid local path: `rfs WRITE data1/localfoo.txt folder/foo.txt`
    - valid: `rfs WRITE data/localfoo.txt another_folder/foo.txt`
- GET
    - valid with existing local path: `rfs GET folder/foo.txt data/localfoo.txt`
    - valid with new local path: `rfs GET folder/foo.txt data2/localfoo.txt`
    - invalid remote path: `rfs GET folder1/foo.txt data/localfoo.txt`
    - valid with a new local path: `rfs GET folder/foo.txt data3/localfoo.txt`
- RM
    - valid: `rfs RM folder/foo.txt`
    - valid: `rfs RM folder/` or `rfs RM folder`
    - invalid remote path: `rfs RM test/data.txt`
    - valid: `rfs RM /`
- Multiple Clients(keep server running and no need to start the client)
    - `./concurrent_test.sh`
    This script launches two clients:
    - The first client writes a large file to the server.
    - The second client attempts to read the same file concurrently (and should wait if necessary due to file locks).