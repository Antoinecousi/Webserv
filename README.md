# WEBSERV

## Overview

This project is part of the 42 school curriculum. The goal is to implement a web server in C++ which is compatible with the HTTP/1.1 protocol. It should be capable of hosting multiple websites and handling various types of requests.

## Features

-   HTTP/1.1 compliance
-   Handling GET, POST, and DELETE requests
-   Ability to host multiple virtual hosts (websites)
-   Customizable server configuration via an nginx configuration file
-   Support for serving static files
-   Error handling with custom error pages

## Prerequisites

-   C++98 compiler
-   Unix-like operating system

## Installation

1. Clone the repository: `git clone git@github.com:Antoinecousi/Webserv.git webserv`
2. Navigate to the project directory: `cd webserv`
3. Compile the project: `make`

## Usage

1. Run the server: `./webserv [configuration_file]` or `./webserv` for default config file.
2. Access the server in a web browser or using a tool like `curl`.

## Configuration File Format

-	It is based on the nginx configuration file format.
-   Listen directive: Specify the IP and port (e.g., `listen 127.0.0.1:8080`).
-   Server name directive: Define server names for virtual hosting.
-   Location blocks: Specify how different URIs should be treated.
