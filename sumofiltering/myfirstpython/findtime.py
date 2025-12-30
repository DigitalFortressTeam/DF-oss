def findrealtime(timegiven):
    thingbetweenhoursandminutes = timegiven.find(":")
    hours = int(timegiven[0:thingbetweenhoursandminutes])
    minutes = int(timegiven[thingbetweenhoursandminutes + 1:thingbetweenhoursandminutes + 3])
    timeinminutes = hours * 60 + minutes
    return timeinminutes
