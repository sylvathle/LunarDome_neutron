#!/bin/bash

cd /home/sylvain/LunarDome_neutron/build
source /home/sylvain/geant/v11.1.1/geant4-v11.1.1-install/bin/geant4.sh
#export PHANTOM_PATH=/home/sylvain/geant/v11.1.1/geant4-v11.1.1/examples/advanced/ICRP145_HumanPhantoms/build/ICRP145data/

# Initialize the control file ".r" with "1" if it doesn't exist
echo "1" > .r

# Specify the file path
file_path="card.par"

# Function to check if a process with a specific argument is already running
is_process_running() {
  local process_argument="$1"
  pgrep -f "./sim macros/run_$process_argument.mac" >/dev/null
  #echo $process_argument $?
  return $?
}

# Function to check if the control file ".r" contains "1"
is_control_file_valid() {
  local control_value
  control_value=$(<.r)
  if [ "$control_value" == "1" ]; then
    return 0  # Control file is valid
  else
    return 1  # Control file is not valid
  fi
}

while is_control_file_valid; do
  # Initialize variables
  nprocess=""
  lines=()
  while IFS= read -r line; do
    if [[ $line == "nprocess "* ]]; then
       nprocess="${line#nprocess }"
    elif [[ -n "$line" && "$line" != "#"* ]]; then
      lines+=("$line")
    fi
  done < "$file_path"

  # Check if there are lines to choose from
  if [ ${#lines[@]} -eq 0 ]; then
    echo "No valid lines found in the file."
    exit 1
  fi

  # Get a random line
  random_line="${lines[RANDOM % ${#lines[@]}]}"
  
  # Split the line into separate variables
  read -r domeParam thick nsim <<< "$random_line"


  nrunning=$(pgrep -c -f "./sim")
  sleep 2

  if [ $nrunning -ge $nprocess ]; then # || [ 1 -gt $navailable ] || [ 10 -gt $memfree ]; then
	  continue
  fi


  cd ../macros/
  python3 genRunMacro.py $domeParam $thick $nsim
  cd ../build/
  echo ./sim ../macros/$thick-$domeParam.mac &
  ./sim ../macros/$thick-$domeParam.mac &
done
