#!/bin/bash

# Define the number of threads, number of pings, and the duration of the iperf test
DEFAULT_NUM_THREADS=1
DEFAUL_NUM_PINGS=5
DEFAUL_SECONDS_IPERF=60

# Command line argument parsing
declare -A ip_map
while getopts a:b:c:d:p:n:l:s: flag
do
    case "${flag}" in
        a) ip_map[IPA]=${OPTARG};;
        b) ip_map[IPB]=${OPTARG};;
        c) ip_map[IPC]=${OPTARG};;
        d) ip_map[IPD]=${OPTARG};;
        p) pid=${OPTARG};;
        n) NUM_THREADS=${OPTARG};;
        l) NUM_PINGS=${OPTARG};;
        s) SECONDS_IPERF=${OPTARG};;
    esac
done

# Set default values if not provided
NUM_THREADS=${NUM_THREADS:-$DEFAULT_NUM_THREADS}
NUM_PINGS=${NUM_PINGS:-$DEFAUL_NUM_PINGS}
SECONDS_IPERF=${SECONDS_IPERF:-$DEFAUL_SECONDS_IPERF}


# Array of all IPs and labels
ips=(${ip_map[IPA]} ${ip_map[IPB]} ${ip_map[IPC]} ${ip_map[IPD]})
labels=("IPA" "IPB" "IPC" "IPD")

if [ -z "$pid" ]; then
    pid=-1
fi

# Get the hostname IP and name
hostname_ipl=$(hostname -I | awk '{print $1}')
# hostname_name=$(hostname)
hostname_ip=$(curl icanhazip.com)
hostname_name=$(hostname)
# Number of nodes
NUM_NODES=${#ips[@]}


# Base port number to avoid conflicts
base_port=5201

pkill -9 -f iperf
for threads in $(seq 1 $NUM_THREADS); do
thread_port=$((base_port + 100 * threads))
server_port=$thread_port

# Start servers on all other nodes' IPs on specific ports in detached mode
for ip in "${ips[@]}"; do
    counter=0
    if [[ $ip != $hostname_ip && $ip != $hostname_ipl && $pid != $counter ]]; then
        echo "IP, PID, Counter, Hostname IP, Hostname IPL: $ip, $pid, $counter, $hostname_ip, $hostname_ipl"
        iperf -s -p $server_port -D
        echo "Server started on $ip:$server_port"
    fi
        ((server_port++))
        ((counter++))
done

done
# Sleep briefly to ensure servers are up
sleep 5

offset=0
for ip in "${ips[@]}"; do
    if [[ $ip == $hostname_ip ]]; then
        break
    else
        ((offset++))
    fi
done

# Start clients to connect to every other server with a test duration of 30 seconds
for threads in $(seq 1 $NUM_THREADS); do
thread_port=$((base_port + 100 * threads))
client_port=$((thread_port + offset))
for ip in "${ips[@]}"; do
    counter=0
    if [[ $ip != $hostname_ip && $ip != $hostname_ipl && $pid != $counter ]]; then
        iperf -c $ip -p $client_port -P 1 -t $SECONDS_IPERF > output_${ip}_${client_port}_threads_${threads}.txt &
        echo "Client started to $ip:$client_port"
    fi
    ((counter++))
done

done
# Wait for all background processes to finish
wait

sleep 1

# Ping each node and capture the average RTT
echo "Ping results and Bandwidth measurements:"
for i in "${!ips[@]}"; do
    ip=${ips[$i]}
    label=${labels[$i]}
    counter=0
    if [[ $ip != $hostname_ip && $ip != $hostname_ipl && $pid != $counter ]]; then
        # Get average round trip time from ping results
        avg_rtt=$(ping -c $NUM_PINGS $ip | tail -1 | awk -F '/' '{print $5}')
        echo "$hostname_name -> $label"
        echo "average RTT: $avg_rtt ms"

        # Extract the four lines from iperf output but only test first two lines of those are needed
        for threads in $(seq 1 $NUM_THREADS); do
        thread_port=$((base_port + 100 * threads))
        client_port=$((thread_port + offset))
        output_file=output_${ip}_${client_port}_threads_${threads}.txt
        sender_bandwidth=$(cat $output_file | grep sender | head -n 1)
        receiver_bandwidth=$(cat $output_file | grep receiver | head -n 1)
        echo "Threads: $threads"
        echo "$sender_bandwidth"
        echo "$receiver_bandwidth"
        done
        
    fi
    ((counter++))
done
