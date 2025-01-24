obj = parameter[1]["data"]  # Access the data object
arrlgt = parameter[1]["count"]["total"] - 1  # Get the last data index
rain_status = obj[arrlgt]["Raining"]  # Get the rain status (1 for rain, 0 for no rain)
threshold = 0

if int(rain_status) > threshold:  # Check if it's raining
    msgbody = '<p>Rain has been detected. Will start the drying fan and retract the clothes line'+'.</p><br>'
    output[1] = "[Warning] Rain Detected! "
    output[2] = msgbody
    output[3] = 2
