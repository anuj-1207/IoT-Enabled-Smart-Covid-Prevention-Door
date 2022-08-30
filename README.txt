CS698T-Introduction to Internet of Things and its Industrial Applications Project 2021

#########################################################################################################################################################################

This project is done by Ankit Raja, Anuj Shrivastava, Himanshu Lal, and Manish.

#########################################################################################################################################################################

Libraries requiured:
Tensorflow: version 2.3.0
Keras: version 2.4.0
opencv: version 4.0.1
numpy: version 1.20.1 
pandas: version 1.2.4
matplotlib: version 3.3.4
notebook: version 1.0.0

#########################################################################################################################################################################

For simplicity, contents are divided into directories
1) Code: This directory contains all the code used for training and detection of model
2) Dataset: This directory contains all the datasets that were used for training the model.
3) Model: This directory contain the model that were used for face mask detection

#########################################################################################################################################################################

To train the model, run "MobileNet_Model.ipynb" File.
To use the face mask classification, run "Face Mask Detection.ipynb" File.

#########################################################################################################################################################################

All the files were stored in ipynb file
1.) Data_Preprocessing.ipynb: This file is used for splitting the Kaggle dataset into train, validation, and test set.
link to Kaggle dataset - https://www.kaggle.com/omkargurav/face-mask-dataset
Load the Kaggle dataset in the folder name data and run this file for splitting it. Use the path written in the notebook for proper functioning.

2) Face Mask Detection.ipynb: This file stores the code to run the classification model. Check the model is present in the model folder before running it

3) Inception_Model.ipynb: This file store the code that was used to train the inception model.

4)VGG16_Model.ipynb: This file store the code that was used to train the vgg16 model.

5)ResNet_Model.ipynb: This file store the code that was used to train the resnet50 model.

6)MobileNet_Model.ipynb: This file store the code that was used to train the mobilenetv3 model.

#########################################################################################################################################################################

Model->deploy.prototxt: This model is used in OpenCV dnn face detection model.
Model->res10_300x300_ssd_iter_140000.caffemodel: This model is used in OpenCV dnn face detection model.
Model->model_mobilenetv2.h5: This model is used for face mask classification

#########################################################################################################################################################################




To simulate this project use this link https://wokwi.com/arduino/projects/315399905058226754


For understanding the execution flow you can look at report on "Figure 6:  Execution sequence"

For entering inside room:-

1. Press on the button "Mask detected".


2. Set the temperature on DHT22 sensor less than 37.5℃ .


3. On the ultrasonic sensor(one below DHT22 Sensor) first set distance less than 15 cm and then more than 15 cm for sanitising hands, less than 15 cm means you are sanitizing your hands and above 15 cm means you sanitized.


4. After doing all the above 3 steps you satisfied all the criterias and hence door will be opened.

5. Once the door opens now comes the ultrasonic sensor at the right of servo motor set its distance below 150 cm and then above 150 cm to finally enter inside the room and make count value increase by one.


For exiting from the room:-

1. The ultrasonic sensor which is at the right of LCD set its distance below 150 cm and then above 150 cm to exit from the room and make count value decrease by one.

Note :- If at any point of time, person returned by doing half process, then there is a specified time for which the system will wait after which timeout will occur and buzzer will beep for 4-5 seconds and then system will reset.



























