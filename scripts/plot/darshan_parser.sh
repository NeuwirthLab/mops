#!/bin/bash
#
## Function to process .darshan files in a directory
#process_directory() {
#    local dir="$1"
#    echo "inn"
#    # Loop through each .darshan file in the directory
#    for file in "$dir"/*.darshan; do
#        # Check if the file is a regular file
#        if [ -f "$file" ]; then
#            # Extract the filename without extension
#            file_name=$(basename "$file" .darshan)
#
#            # Execute darshan-parser command and redirect output to a text file with .posix extension
#            darshan-parser --total --perf "$file" > "$dir/$file_name.posix"
#        fi
#    done
#}
#
## Check if the directory path is provided as an argument
#if [ $# -ne 1 ]; then
#    echo "Usage: $0 <directory_path>"
#    exit 1
#fi
#
#export DYLD_LIBRARY_PATH=/usr/lib:$DYLD_LIBRARY_PATH
## Set the directory path from the argument
#directory_path="$1"
#
## Check if the directory exists
#if [ ! -d "$directory_path" ]; then
#    echo "Error: Directory '$directory_path' not found."
#    exit 1
#fi
#
## Process subdirectories recursively
#while IFS= read -r -d '' subdirectory; do
#    process_directory "$subdirectory"
#done < <(find "$directory_path" -mindepth 1 -type d -print0)
#
#echo "All .darshan files processed."


# Function to process .darshan files in a directory
process_directory() {
    local dir="$1"
    echo "Processing files in directory: $dir"
    # Loop through each .darshan file in the directory
    for file in "$dir"/*.darshan; do
        # Check if the file is a regular file
        if [ -f "$file" ]; then
            # Extract the filename without extension
            file_name=$(basename "$file" .darshan)

            # Execute darshan-parser command and redirect output to a text file with .posix extension
            darshan-parser --total --perf "$file" > "$dir/$file_name.posix"
        fi
    done
}

# Check if the directory path is provided as an argument
if [ $# -ne 1 ]; then
    echo "Usage: $0 <directory_path>"
    exit 1
fi

export DYLD_LIBRARY_PATH=/usr/lib:$DYLD_LIBRARY_PATH
# Set the directory path from the argument
directory_path="$1"

# Check if the directory exists
if [ ! -d "$directory_path" ]; then
    echo "Error: Directory '$directory_path' not found."
    exit 1
fi

# Process files in the specified directory
process_directory "$directory_path"

echo "All .darshan files processed."