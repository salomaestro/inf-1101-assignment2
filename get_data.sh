#!/bin/bash

big_file="data/trump.txt"
out_file_prefix="data/trump_tweet_"

# Steal some Trump tweets
if ! test -f $big_file; then
  wget -P data/ https://raw.githubusercontent.com/harshilkamdar/trump-tweets/master/trump.txt
fi

# Get the total number of lines in the input file
num_lines=$(wc -l < "$big_file")

# Set the number of output files you want to create
num_output_files=$1

# Loop through each output file
for (( i=1; i<=$num_output_files; i++ ))
do
  # Generate a random line number between 1 and the number of lines in the input file
  random_line_num=$(( RANDOM % $num_lines + 1 ))
  
  # Get the random line from the input file
  random_line=$(sed "${random_line_num}q;d" "$big_file")
  
  # Write the random line to the output file
  output_file="${out_file_prefix}${random_line_num}.txt"
  echo "$random_line" > "$output_file"
done

# if test -f $big_file; then
#   rm $big_file
# fi
