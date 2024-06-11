# InodeInspector

gcc -o inspect inspect.c  

My implementation to adhere for the following goals:  

Learn to access file metadata using system calls in C.  
Understand inode properties and their roles in the filesystem.  
Develop a functional CLI tool for real-world application.  
Requirements:  
Core Functionality:  
  
Retrieve and display inode information for specified files.  
Output information in both detailed and human-readable formats.  
Support multiple output formats: JSON and plain text.  
Implement a command-line interface with comprehensive options for user interaction.  
Command-Line Options:  
  
-?, --help: Display help information about the tool and its options.  
Example: inspect -?  
Note that this is also the default behavior if the user types the command inspect without any arguments.  
-i, --inode <file_path>: Display detailed inode information for the specified file.  
Example: inspect -i /path/to/file  
Note that this flag is optional and the default behavior is identical if the flag is omitted.  
-a, --all [directory_path]: Display inode information for all files within the specified directory. If no path is provided, 
 default to the current directory.  
Optional flag: -r, --recursive for recursive listing.  
Example: inspect -a /path/to/directory -r  
-h, --human: Output all sizes in kilobytes (K), megabytes (M), or gigabytes (G) and all dates in a human-readable form.  
Example: inspect -i /path/to/file -h  
-f, --format [text|json]: Specify the output format. If not specified, default to plain text.  
Example: inspect -i /path/to/file -f json  
-l, --log <log_file>: Log operations to a specified file.  
Example: inspect -i /path/to/file -l /path/to/logfile  
Error Handling:  
  
The tool must handle and report errors effectively, such as invalid file paths, insufficient permissions, and unsupported  
 file types.  
