/*  Copyright 2011 AIT Austrian Institute of Technology
*
*   This file is part of OpenTLD.
*
*   OpenTLD is free software: you can redistribute it and/or modify
*   it under the terms of the GNU General Public License as published by
*    the Free Software Foundation, either version 3 of the License, or
*   (at your option) any later version.
*
*   OpenTLD is distributed in the hope that it will be useful,
*   but WITHOUT ANY WARRANTY; without even the implied warranty of
*   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
*   GNU General Public License for more details.
*
*   You should have received a copy of the GNU General Public License
*   along with OpenTLD.  If not, see <http://www.gnu.org/licenses/>.
*
*/
/*
 * ForegroundDetector.cpp
 *
 *  Created on: Nov 16, 2011
 *      Author: Georg Nebehay
 */

#include "ForegroundDetector.h"

#include "BlobResult.h"

using namespace cv;

namespace tld
{

ForegroundDetector::ForegroundDetector()
{
    fgThreshold = 16;
    minBlobSize = 0;
}

ForegroundDetector::~ForegroundDetector()
{
}

void ForegroundDetector::release()
{
}

void ForegroundDetector::nextIteration(const Mat &img)
{
    if(bgImg.empty())
    {
        return;
    }

    Mat absImg = Mat(img.cols, img.rows, img.type());
    Mat threshImg = Mat(img.cols, img.rows, img.type());

    absdiff(bgImg, img, absImg); //eq 3.2
    threshold(absImg, threshImg, fgThreshold, 255, CV_THRESH_BINARY); //eq 3.3 threshold = 16

    IplImage im = (IplImage)threshImg;
    CBlobResult blobs = CBlobResult(&im, NULL, 0); // labelling 

    blobs.Filter(blobs, B_EXCLUDE, CBlobGetArea(), B_LESS, minBlobSize);

    std::vector<Rect>* fgList = detectionResult->fgList;
    fgList->clear();

    for(int i = 0; i < blobs.GetNumBlobs(); i++)
    {
        CBlob *blob = blobs.GetBlob(i);
        CvRect rect = blob->GetBoundingBox(); // bounding box
        fgList->push_back(rect); // store bounding box
    }

}

bool ForegroundDetector::isActive()
{
    return !bgImg.empty();
}

} /* namespace tld */
