ADCompVision area detector plugin Input/output manual/guide
===========================================================

:author: Jakub Wlodek, Brookhaven National Laboratory

:date: November 21, 2018

:updated: June 25, 2019

| `Return <ADCompVision.html>`__

.. contents:: Contents

Introduction
------------

This is a manual that documents the input and output types of each
WRAPPER function supported by ADCompVision

Wrapper function inputs/outputs
-------------------------------

Below is a table that details the inputs and outputs used by each
function, along with descriptions for each function

.. raw:: html

  <table border="1" cellpadding="2" cellspacing="2" style="text-align: left">
    <tbody>
      <tr>
        <td align="center" colspan="7">
          <b>Input/Output values for each ADCompVision Function</b>
        </td>
      </tr>
      <tr>
      <th>
        Function Name</th>
      <th>
        Num inputs</th>
      <th>
        Input value format</th>
      <th>
        Num outputs</th>
      <th>
        Output value format</th>
  </tr><tr>
  <th>GaussianBlur
  </th>
  <th>1
  </th>
  <th>[blurDegree (Int)]
  </th>
  <th>0
  </th>
  <th>N/A
  </th>
  </tr>
  <tr>
  <th>ThresholdImage
  </th>
  <th>3
  </th>
  <th>[Threshhold Value (Int), Max Pixel Value (Int)]
  </th>
  <th>0
  </th>
  <th>N/A
  </th>
  </tr>
  <tr>
  <th>LaplacianEdgeDetector
  </th>
  <th>1
  </th>
  <th>[Blur degree (Int)]
  </th>
  <th>0
  </th>
  <th>None
  </th>
  </tr>
  <tr>
  <th>Sharpen
  </th>
  <th>4
  </th>
  <th>[Gaussian blurr (Int), Laplacian kernel size (Int), Laplacian scale (Int), Laplacian delat (Int) ]
  </th>
  <th>0
  </th>
  <th>N/A
  </th>
  </tr>
  <tr>
  <th>CannyEdgeDetector
  </th>
  <th>3
  </th>
  <th>[Threshold value (Int), Threshold ratio (Int), Blur degree (Int), Kernel Size (Int)]
  </th>
  <th>8
  </th>
  <th>[Horizontal Center, Horizontal Size, Vertical Center, Vertical Size, Top Pixel, Bottom Pixel, Left Pixel, Right Pixel]
  </th>
  </tr>
  <tr>
  <th>SubtractConsecutiveImages
  </th>
  <th>0
  </th>
  <th>N/A
  </th>
  <th>0
  </th>
  <th>N/A
  </th>
  </tr>
  <tr>
  <th>ComputeImageStats
  </th>
  <th>0
  </th>
  <th>N/A
  </th>
  <th>9
  </th>
  <th>[total, min, min x, min y, max, max x, max y, mean, sigma]
  </th>
  </tr>
  <tr>
  <th>VideoRecord
  </th>
  <th>4
  </th>
  <th>[Framerate (Int), Start/Stop (1 or 0), color (1 or 0), encoding (1-4), Output File Type (1 or 0)]
  </th>
  <th>0
  </th>
  <th>N/A
  </th>
  </tr>
  <tr>
  <th>FindObjectCentroids
  </th>
  <th>5
  </th>
  <th>[Num Largest Contours (Int), Blur Degree (Int), Threshold Value (Int), Upper Size Threshold (Int), Lower Size Threshold (Int)]
  </th>
  <th>2-10
  </th>
  <th>[CentroidX (Int), CentroidY (Int) ... ]
  </th>
  </tr>
  <tr>
  <th>MovementVectors(Testing)
  </th>
  <th>2
  </th>
  <th>[Frames Between Images (Int), Num Vectors (Int)]
  </th>
  <th>0-8
  </th>
  <th>[Vector 1 Start X (Int), Vector 1 Start Y (Int), Vector 1 End X, Vector 1 End Y ...]
  </th>
  </tr>
  <tr>
  <th>ObjectIdentification
  </th>
  <th>4
  </th>
  <th>[Param1 (Int), Param2 (Double) ...]
  </th>
  <th>10
  </th>
  <th>[Param1 (Int), Param2 (Double) ...]
  </th>
  </tr>
  <tr>
  <th>UserFunction
  </th>
  <th>n
  </th>
  <th>[Param1 (Int), Param2 (Double) ...]
  </th>
  <th>n
  </th>
  <th>[Param1 (Int), Param2 (Double) ...]
  </th>
  </tr>
  <tr>
  <th>DistanceBetweenContours
  </th>
  <th>5
  </th>
  <th>[Distance Threshold (Int), Blur Kernel Size (Int), Threshold (Int), Apply Blur (Toggle), Pixel Size Threshold (Int)]
  </th>
  <th>2
  </th>
  <th>[Is Within Threshold (Binary Int), Distance in Pixels (Int)]
  </th>
  </tr>
  <tr>
  <th>ConvertImageFormat
  </th>
  <th>2
  </th>
  <th>[To grayscale (Toggle), To rgb (Toggle)]
  </th>
  <th>0
  </th>
  <th>N/A
  </th>
  </tr>

  </tbody>
  </table>

Function descriptions
---------------------

GaussianBlur
~~~~~~~~~~~~~~

Blurs image based on a gaussian kernel. A gaussian kernel is simply a
matrix of a set size that fills Gaussian properties.

ThresholdImage
~~~~~~~~~~~~~~

Function that thresholds an image based on a certain pixel value. First,
the image is converted to grayscale. RGB images cannot be thresholded.
For each pixel, if the grayscale value is larger than the threshold, set
it to white, otherwise set it to black. Creates a binary image

LaplacianEdgeDetector
~~~~~~~~~~~~~~~~~~~~~

Function for laplacian-based edge detection. First, the image is
converted to grayscale if it is not already. Next, the image is blurred
using a gaussian kernel to emphasize edges. Then a laplacian kernel runs
over the images assigning a 'sharpness' value to each pixel. The
sharpest values are hard edges from black to white.

Sharpen
~~~~~~~

Sharpens image by sutracting Laplacian from blurred image

CannyEdgeDetector
~~~~~~~~~~~~~~~~~

Function for canny-based edge detection. First, we ensure that the image
is grayscale. Then, the image is blurred, so that only strong edges
remain. Then, a threshold is applied to the image in order to further
reinforce strong edges. Finally, the canny algorithm is applied to the
image, and the edges are displayed. The function outputs some
information based on the detected edges that can assist with object
detection/identification: the top, bottom, left, and right pixels are
the min and max X and Y pixel values that appear on one of the edges.
The horizontal and vertical size and center give you the spacing between
these min and max values and their midpoint.

SubtractConsecutiveImages
~~~~~~~~~~~~~~~~~~~~~~~~~

Function that allows the user to take consecutive images recieved from
area detector and subtract them in pairs. Reads first image into memory,
then waits for second one, when it receives the second one, subtract
them.

ComputeImageStats
~~~~~~~~~~~~~~~~~~~~~

OpenCV accelerated computation of Image statistics

VideoRecord
~~~~~~~~~~~

This function uses the opencv_video and opencv_videoio libraries for writing a video from areaDetector cameras.
A valid file path is required. Output video framerate should be set to the camera framerate if a real time video
is desired. Supported encodings are: H264, MPEG, DIVX, and LAGS. Not all encodings will be present on each machine,
thus some experimentation may be required. The output video will $FILEPATH/CV\_Output\_Vid\_$DATETIME.mp4

FindObjectCentroids
~~~~~~~~~~~~~~~~~~~

Function for finding centroids of objects in an image. Useful for
alignment of objects First, blur the object based on a certain blur
degree (kernel size). Then threshold the image based on a certain
threshold value. Then find contours in the image using the
findContours() function. Then get the centroids from the contour
objects. Draw the contours and centroids on the image. Set the first 5
centroid coordinates to the output values. A size filter can also be
used to remove contours that are too large, removing contours that span
the entire size of the image. Any contour with area > upper threshold is
removed, and any lower than lower threshold

MovementVectors (Testing)
~~~~~~~~~~~~~~~~~~~~~~~~~

Function that does feature detection on images a set number of frames
apart, and attempts to calculate the movement vector for the calculated
key points. It uses ORB feature detection and vector flow NOT YET
IMPLEMENTED/TESTED

ObjectIdentification (Testing)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

Function that detects contours in an image and returns information
regarding said contours NOT YET IMPLEMENTED/TESTED

UserFunction
~~~~~~~~~~~~

This is an unimplemented wrapper function that has already been added to
the PV database in order to simplify creating user defined functions.
Simply implement this function and its description function, and then
select 'User Function' in function set 3.

Distancebetweencontours
~~~~~~~~~~~~~~~~~~~~~~~

Function that computes bounding boxes between the two largest computed
contours in the image, checks the distance between them and sends an
alarm if they are within a distance threshold.

ConvertImageFormat
~~~~~~~~~~~~~~~~~~

Converts the format of the image to a different one for use with other AD Plugins. This is useful for
cameras that only support one format but a different one is required, ex. ADPluginDmtx needs 8bit rgb image, so
grayscale camera needs to be converted.
