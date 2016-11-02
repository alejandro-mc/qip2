// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// BoxFilter.h - BoxFilter widget
//
// Written by: George Wolberg, 2016
// ======================================================================

#ifndef BoxFilter_H
#define BoxFilter_H

#include "ImageFilter.h"

class BoxFilter : public ImageFilter {
	Q_OBJECT

public:
    BoxFilter				(QWidget *parent = 0);		// constructor
	QGroupBox*	controlPanel	();				// create control panel
	bool		applyFilter	(ImagePtr, bool, ImagePtr);	// apply filter to input
	void		reset		();				// reset parameters
    void		boxfilter	(ImagePtr, int, int, ImagePtr);
	void		initShader();
	void		gpuProgram(int pass);	// use GPU program to apply filter

protected slots:
	void		changeFilterW	(int);
	void		changeFilterH	(int);
	void		setLock		(int);

private:
	// widgets
    QSlider*	m_slider [2];	// BoxFilter sliders
    QSpinBox*	m_spinBox[2];	// BoxFilter spin boxes
    QCheckBox*	m_checkBox;	    // BoxFilter check box
    QGroupBox*	m_ctrlGrp;	    // groupbox for panel
    int		m_width;	        // input image width
    int		m_height;	        // input image height
    float   m_filter[900];      // filter array
};

#endif	// BoxFilter_H
