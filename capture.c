#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/videodev2.h>

#define VIDEO_DEVICE "/dev/video0"

int main(void)
{
    printf("Opening Device "VIDEO_DEVICE".\n");

    int fd = open(VIDEO_DEVICE, O_RDWR);
    if(-1 == fd)
    {
        perror("Error Opening Video Device "VIDEO_DEVICE);
        exit(EXIT_FAILURE);
    }

    printf("\nChecking device capabilities.\n");

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

    printf("\nGetting Video Formats\n");
    return 0;
}
