#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/videodev2.h>

#define VIDEO_DEVICE "/dev/video0"

int main(void)
{
    printf("Opening Device "VIDEO_DEVICE".\n");

    int fd = open(VIDEO_DEVICE, O_RDWR);
    if(-1 == fd)
    {
        perror("Error Opening Video Device "VIDEO_DEVICE".");
        exit(EXIT_FAILURE);
    }

    printf("\nChecking device capabilities.\n");

    // https://docs.kernel.org/userspace-api/media/v4l/vidioc-querycap.html#vidioc-querycap
    struct v4l2_capability caps = {0};
    if(-1 == ioctl(fd, VIDIOC_QUERYCAP, &caps))
    {
        perror("Error Querying Capabilities");
        exit(EXIT_FAILURE);
    }
    printf("Driver: %s\nCard %s\nBus: %s\nVersion: %d\n",
           caps.driver,
           caps.card,
           caps.bus_info,
           caps.version
        );
    // Other capabilities are in the caps.capabilities field.
    printf("Capabilities: 0x%x\n"
           "\tVideo Capture Bit: %u\n"
           "\tStreaming: %u\n",
           caps.capabilities,
           !!(caps.capabilities & V4L2_CAP_VIDEO_CAPTURE),
           !!(caps.capabilities & V4L2_CAP_STREAMING)
        );

    printf("\nGetting Video Formats\n");
    // https://docs.kernel.org/userspace-api/media/v4l/vidioc-g-fmt.html#vidioc-g-fmt
    struct v4l2_format format = {.type = V4L2_BUF_TYPE_VIDEO_CAPTURE};
    if(-1 == ioctl(fd, VIDIOC_G_FMT, &format))
    {
        perror("Error Querying Formats.");
        exit(EXIT_FAILURE);
    }

    printf("\tWidth: %d Height %d\n", format.fmt.pix.width, format.fmt.pix.height);

    printf("Meta Info\n");
    printf("\tBuffer size: %d\n", format.fmt.meta.buffersize);
    printf("\tData format: %d\n", format.fmt.meta.dataformat);

    printf("Pixel Info\n");
    printf("\tBytesPerLine %d\n", format.fmt.pix.bytesperline);
    printf("\tColor Space  %d\n", format.fmt.pix.colorspace);
    printf("\tField %d\n", format.fmt.pix.field);
    printf("\tFlags %d\n", format.fmt.pix.flags);
    printf("\tPix Height %d Width %d\n", format.fmt.pix.height, format.fmt.pix.width);
    printf("\tHSV Enc %d\n", format.fmt.pix.hsv_enc);
    printf("\tPixelformat %d\n", format.fmt.pix.pixelformat);
    printf("\tPriv %d\n", format.fmt.pix.priv);
    printf("\tQuantization %d\n", format.fmt.pix.quantization);
    printf("\tSizeImage %d\n", format.fmt.pix.sizeimage);
    printf("\tXfer_func %d\n", format.fmt.pix.xfer_func);
    printf("\tycbcr_enc %d\n", format.fmt.pix.ycbcr_enc);

    printf("Pix mp Info\n");
    printf("\tColor Space  %d\n", format.fmt.pix_mp.colorspace);
    printf("\tField %d\n", format.fmt.pix_mp.field);
    printf("\tFlags %d\n", format.fmt.pix_mp.flags);
    printf("\tPix Height %d Width %d\n", format.fmt.pix_mp.height, format.fmt.pix_mp.width);
    printf("\tHSV Enc %d\n", format.fmt.pix_mp.hsv_enc);
    printf("\tPixelformat %d\n", format.fmt.pix_mp.pixelformat);
    printf("\tPlane_fmt bpl %d\n", format.fmt.pix_mp.plane_fmt->bytesperline);
    // printf("\tPlane_fmt res %d\n", format.fmt.pix_mp.plane_fmt->reserved);
    printf("\tPlane_fmt sizeimage %d\n", format.fmt.pix_mp.plane_fmt->sizeimage);
    printf("\tQuantization %d\n", format.fmt.pix_mp.quantization);
    // printf("\tReserved %d\n", format.fmt.pix_mp.reserved);
    printf("\tXfer_func %d\n", format.fmt.pix_mp.xfer_func);
    printf("\tycbcr_enc %d\n", format.fmt.pix_mp.ycbcr_enc);
    printf("\tNum Planes %d\n", format.fmt.pix_mp.num_planes);

    printf("Raw Data\n");
    printf("%s\n", format.fmt.raw_data);

    printf("SDR\n");
    printf("\tBuffer Size: %d\n", format.fmt.sdr.buffersize);
    printf("\tPix Format: %d\n", format.fmt.sdr.pixelformat);
    // printf("\tReserved: %d\n", format.fmt.sdr.reserved);

    printf("Sliced\n");
    printf("\tIO size %d\n", format.fmt.sliced.io_size);
    printf("\tService lines %d\n", format.fmt.sliced.service_lines);
    printf("\tService set %d\n", format.fmt.sliced.service_set);

    printf("VBI\n");
    printf("\tCount: %ls\n", format.fmt.vbi.count);
    printf("\tFlags: %d\n", format.fmt.vbi.flags);
    printf("\tOffset: %d\n", format.fmt.vbi.offset);
    printf("\tSample fmt: %d\n", format.fmt.vbi.sample_format);
    printf("\tsamples per line: %d\n", format.fmt.vbi.samples_per_line);
    printf("\tsample rate: %d\n", format.fmt.vbi.sampling_rate);
    printf("\tStart: %ls\n", format.fmt.vbi.start);

    printf("Win\n");
    printf("\tchromakey %d\n", format.fmt.win.chromakey);
    printf("\tclipcount %d\n", format.fmt.win.clipcount);
    // printf("\tclips c %d\n", format.fmt.win.clips->c);
    printf("\tfield %d\n", format.fmt.win.field);
    printf("\tglobal alpha %d\n", format.fmt.win.global_alpha);
    printf("\tw height %d\n", format.fmt.win.w.height);
    printf("\tw width %d\n", format.fmt.win.w.width);
    printf("\tw top %d\n", format.fmt.win.w.top);
    printf("\tw left %d\n", format.fmt.win.w.left);


    char pixelformat[5]="";
    pixelformat[3] = format.fmt.pix.pixelformat >> 24;
    pixelformat[2] = (format.fmt.pix.pixelformat & 0x00FF0000) >> 16;
    pixelformat[1] = (format.fmt.pix.pixelformat & 0x0000FF00) >> 8;
    pixelformat[0] = format.fmt.pix.pixelformat & 0x000000FF;
    printf("\n\tPixel Format: 0x%x (%s)\n", format.fmt.pix.pixelformat, pixelformat);

    // Rather than setting a format we will use the one the camera is currently set to

    // The next step is to request that the device allocate a buffer for streaming
    printf("\nRequesting Buffer for MMAP Streaming\n");
    // There are other streaming methods to try but
    // mmap streaming seems to be the most common
    // https://docs.kernel.org/userspace-api/media/v4l/vidioc-reqbufs.html#vidioc-reqbufs
    struct v4l2_requestbuffers req =
        {
            .count = 1,
            .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
            .memory = V4L2_MEMORY_MMAP,
        };
    if(-1 == ioctl(fd, VIDIOC_REQBUFS, &req))
    {
        perror("Error Requesting Buffers.");
        exit(EXIT_FAILURE);
    }

    // map the buffer into memory so this process can access it
    struct v4l2_buffer buf_info =
        {
            .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
            .memory = V4L2_MEMORY_MMAP,
            .index = 0
        };

    if(-1 == ioctl(fd, VIDIOC_QUERYBUF, &buf_info))
    {
        perror("Error Querying Buffer");
        exit(EXIT_FAILURE);
    }
    printf("Buffer size is %d\n", buf_info.length);

    printf("Allocated the memory-mapped buffer\n");
    void * buffer = mmap(NULL,
                         buf_info.length,
                         PROT_READ | PROT_WRITE,
                         MAP_SHARED,
                         fd,
                         buf_info.m.offset);

    if(MAP_FAILED == buffer)
    {
        perror("Error mapping memory");
        exit(EXIT_FAILURE);
    }
    // clear the buffer to be safe
    memset(buffer, 0, buf_info.length);

    // Now ready to start the stream and grab a frame
    printf("Beginning the stream\n");
    u_int32_t type = buf_info.type;
    if(-1 == ioctl(fd, VIDIOC_STREAMON, &type))
    {
        perror("Could not begin stream.");
        exit(EXIT_FAILURE);
    }

    // With streaming active we can now grab a frame
    printf("Enqueueing a buffer.\n");
    // Enqueue the buffer. Make a new struct so it is cleared
    struct v4l2_buffer buf_info1 =
        {
            .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
            .memory = V4L2_MEMORY_MMAP,
            .index = 0
        };
    if(-1 == ioctl(fd, VIDIOC_QBUF, &buf_info1))
    {
        perror("Could not enqueue buffer");
        exit(EXIT_FAILURE);
    }

    printf("Waiting for data\n");

    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(fd, &fds);
    struct timeval timeout = { .tv_sec = 2 };
    if( -1 == select(fd + 1, &fds, NULL, NULL, &timeout))
    {
        perror("Error waiting for frame.");
        exit(EXIT_FAILURE);
    }


    printf("Reading the frame\n");
    struct v4l2_buffer buf1 =
        {
            .type = V4L2_BUF_TYPE_VIDEO_CAPTURE,
            .memory = V4L2_MEMORY_MMAP
        };

    // Grab the frame
    if(-1 == ioctl(fd, VIDIOC_DQBUF, &buf1))
    {
        perror("Error grabbing frame.");
        exit(EXIT_FAILURE);
    }


    printf("Creating temporary output file\n");
    // write the frame to a temporary file
    char tmpfile_name[]="video-XXXXXX";
    int tmpfile = mkstemp(tmpfile_name);
    if(-1 == tmpfile )
    {
        perror("Error creating temporary output file.");
        exit(EXIT_FAILURE);
    }
    printf("Writing frame to %s\n", tmpfile_name);
    if( -1 == write(tmpfile, buffer, buf_info1.length))
    {
        perror("Error writing to tmpfile.");
        exit(EXIT_FAILURE);
    }
    printf("Frame capture complete.\n");

    printf("To view the frame (which is a single still image convert it to an image file using ffmpeg.\n");
    printf("The exact commands depend on the color space and resolution (both of which are printed by this program");
    printf("The command is: ffmpeg -f rawvideo -pix_fmt <format> -s:v <width>x<height> -i video-XXXXXX output.mp4\n");
    printf("To view the movie (which is one frame long): ffplay output.mp4\n");
    printf("ffplay seems to repeat the movie by default\n");
    printf("<width> and <height> need to be the actual width and heigh tof the image\n");
    printf("<format> is the pixel format.  For a YUYV camera, use yuyv422.\n");
    printf("<format> can be mjpeg for MJPG.\n");
    printf("Theoretically, the frame from MJPG is already a JPG image, so you can see it in any JPG viewer without conversion.\n");
    return 0;
}
