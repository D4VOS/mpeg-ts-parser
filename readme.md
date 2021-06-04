# MPEG-TS Parser

> MPEG transport stream is a standard digital container format for transmission and storage of audio, video, and Program and System Information Protocol (PSIP) data.
> It is used in broadcast systems such as DVB, ATSC and IPTV. [Wikipedia]


## What is it for?
This simple program allows you to parse a `.ts` file (MPEG-TS file). Collects information about all packages and saves them to a text file.
Additionally, it extracts all possible packet streams (such as e.g. audio or video) to separate binaries.


## Variables
- `source_path` - MPEG-TS file path
- `output_path` - output text file path

## Output
- `output.txt` defined in `output_path`
- `PID_X.bin` where `X` is the stream identifier 
> `PID=174` (video) and `PID=136` (audio) files have the file extensions `.264` and `.mp2`



[Wikipedia]:<https://en.wikipedia.org/wiki/MPEG_transport_stream>