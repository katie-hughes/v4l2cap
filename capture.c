#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/ioctl.h>
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
    if(EINVAL == ioctl(fd, VIDIOC_QUERYCAP, &caps))
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
    // Other capabilities are in the caps.capabilities field. We mainly care about video capture
    printf("Capabilities: 0x%x, Video Capture Bit: %d\n",
           caps.capabilities,
           caps.capabilities & V4L2_CAP_VIDEO_CAPTURE);

    printf("\nGetting Video Formats\n");

    struct v4l2_format fmt = {.type = V4L2_BUF_TYPE_VIDEO_CAPTURE};
    if(-1 == ioctl(fd, VIDIOC_G_FMT, &fmt))
    {
        perror("Error Querying Formats.");
        exit(EXIT_FAILURE);
    }


    return 0;
}
