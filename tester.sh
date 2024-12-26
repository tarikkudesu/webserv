#!/bin/bash

# Benchmark mode with 100 users for 1 minute
siege -c 100 -t 1m -b http://localhost:8080
siege -c 100 -t 1m -b http://localhost:8080
siege -c 100 -t 1m -b http://localhost:8081
siege -c 100 -t 1m -b http://localhost:8000
siege -c 100 -t 1m -b http://localhost:8001
siege -c 100 -t 1m -b http://localhost:8002
siege -c 100 -t 1m -b http://localhost:1024
siege -c 100 -t 1m -b http://localhost:9000

# Test with delay and detailed logging
siege -c 50 -d 2 -v -t 2m http://localhost:8080
siege -c 50 -d 2 -v -t 2m http://localhost:8080
siege -c 50 -d 2 -v -t 2m http://localhost:8081
siege -c 50 -d 2 -v -t 2m http://localhost:8000
siege -c 50 -d 2 -v -t 2m http://localhost:8001
siege -c 50 -d 2 -v -t 2m http://localhost:8002
siege -c 50 -d 2 -v -t 2m http://localhost:1024
siege -c 50 -d 2 -v -t 2m http://localhost:9000