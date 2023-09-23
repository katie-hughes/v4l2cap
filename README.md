# Video Capture Using V4l2 API
- Experimental code to read from a webcam using the v4l2 API
- Build with `make`
- Will output a single frame from `/dev/video0` to a file called `video-XXXXXX`,
  where the `XXXXXX` will be random letters and numbers.
- The raw frame may need to be converted (e.g., with ffmpeg) to be viewable.
