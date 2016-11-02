// ======================================================================
// IMPROC: Image Processing Software Package
// Copyright (C) 2016 by George Wolberg
//
// BoxFilter.cpp - BoxFilter widget.
//
// Written by: George Wolberg, 2016
// ======================================================================

#include "MainWindow.h"
#include "BoxFilter.h"
//#include "hw2/HW_BoxFilter.cpp"


#ifndef PI
#define PI 3.141559
#endif

extern MainWindow *g_mainWindowP;
enum { WSIZE, HSIZE,HSTEP,VSTEP, STEP, SAMPLER,FILTER };

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BoxFilter::BoxFilter:
//
// Constructor.
//
BoxFilter::BoxFilter(QWidget *parent) : ImageFilter(parent)
{}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BoxFilter::controlPanel:
//
// Create group box for control panel.
//
QGroupBox*
BoxFilter::controlPanel()
{
	// init group box
    m_ctrlGrp = new QGroupBox("BoxFilter");

	// layout for assembling filter widget
	QGridLayout *layout = new QGridLayout;

	// alloc array of labels
	QLabel *label[2];

	// create sliders and spinboxes
	for(int i=0; i<2; i++) {
		// create label[i]
		label[i] = new QLabel(m_ctrlGrp);
		if(!i) label[i]->setText("Width");
		else   label[i]->setText("Height");

		// create slider
		m_slider [i] = new QSlider(Qt::Horizontal, m_ctrlGrp);
		m_slider [i]->setRange(1, 30);
		m_slider [i]->setValue(3);
		m_slider [i]->setSingleStep(2);
		m_slider [i]->setTickPosition(QSlider::TicksBelow);
		m_slider [i]->setTickInterval(5);

		// create spinbox
		m_spinBox[i] = new QSpinBox(m_ctrlGrp);
		m_spinBox[i]->setRange(1, 30);
		m_spinBox[i]->setValue(3);
		m_spinBox[i]->setSingleStep(2);

		// assemble dialog
		layout->addWidget(label    [i], i, 0);
		layout->addWidget(m_slider [i], i, 1);
		layout->addWidget(m_spinBox[i], i, 2);
	}

	// create checkbox
	m_checkBox = new QCheckBox("Lock filter dimensions");
	m_checkBox->setCheckState(Qt::Checked);

	// add checkbox to layout
	layout->addWidget(m_checkBox, 2, 1, Qt::AlignHCenter);

	// init signal/slot connections
	connect(m_slider [0], SIGNAL(valueChanged(int)), this, SLOT(changeFilterW(int)));
	connect(m_spinBox[0], SIGNAL(valueChanged(int)), this, SLOT(changeFilterW(int)));
	connect(m_slider [1], SIGNAL(valueChanged(int)), this, SLOT(changeFilterH(int)));
	connect(m_spinBox[1], SIGNAL(valueChanged(int)), this, SLOT(changeFilterH(int)));
	connect(m_checkBox  , SIGNAL(stateChanged(int)), this, SLOT(setLock(int)));

	// assign layout to group box
	m_ctrlGrp->setLayout(layout);

	return(m_ctrlGrp);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BoxFilter::applyFilter:
//
// Run filter on the image, transforming I1 to I2.
// Overrides ImageFilter::applyFilter().
// Return 1 for success, 0 for failure.
//
bool
BoxFilter::applyFilter(ImagePtr I1, bool gpuFlag, ImagePtr I2)
{
	// error checking
	if(I1.isNull()) return 0;

	// collect parameters
	int w = m_slider[0]->value();	// filter width
	int h = m_slider[1]->value();	// filter height
	m_width  = I1->width();
	m_height = I1->height();
    // apply BoxFilter filter
	if(!(gpuFlag && m_shaderFlag))
        boxfilter(I1, w, h, I2);	// apply CPU based filter
	else    g_mainWindowP->glw()->applyFilterGPU(m_nPasses);

	return 1;
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BoxFilter::BoxFilter:
//
// BoxFilter image I1 with a box filter (unweighted averaging).
// The filter has width w and height h.
// Output is in I2.
//
void
BoxFilter::boxfilter(ImagePtr I1, int w, int h, ImagePtr I2)
{
//    HW_boxfilter(I1, w, h, I2);
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BoxFilter::changeFilterW:
//
// Slot to process change in filter width caused by moving the slider.
//
void
BoxFilter::changeFilterW(int value)
{
	// set value of m_slider[0] and tie it to m_slider[1] if necessary
	for(int i=0; i<2; i++) {
		m_slider [i]->blockSignals(true);
		m_slider [i]->setValue    (value);
		m_slider [i]->blockSignals(false);
		m_spinBox[i]->blockSignals(true);
		m_spinBox[i]->setValue    (value);
		m_spinBox[i]->blockSignals(false);

		// don't tie slider values if lock checkbox is not checked
		if(m_checkBox->checkState() != Qt::Checked) break;
	}

	// apply filter to source image and display result
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BoxFilter::changeFilterH:
//
// Slot to process change in filter height caused by moving the slider.
//
void
BoxFilter::changeFilterH(int value)
{
	// set value of m_slider[1] and tie it to m_slider[0] if necessary
	for(int i=1; i>=0; i--) {
		m_slider [i]->blockSignals(true);
		m_slider [i]->setValue    (value);
		m_slider [i]->blockSignals(false);
		m_spinBox[i]->blockSignals(true);
		m_spinBox[i]->setValue    (value);
		m_spinBox[i]->blockSignals(false);

		// don't tie slider values if lock checkbox is not checked
		if(m_checkBox->checkState() != Qt::Checked) break;
	}

	// apply filter to source image and display result
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// HistStretchDialog::setLock:
//
// Slot to process state change of "Lock" checkbox.
// Set both sliders to same (min) value.
//
void
BoxFilter::setLock(int state)
{
	if(state == Qt::Checked) {
		int val = MIN(m_slider[0]->value(), m_slider[1]->value());
		for(int i=0; i<2; i++) {
			m_slider[i]->blockSignals(true);
			m_slider[i]->setValue(val);
			m_slider[i]->blockSignals(false);
		}
	}

	// apply filter to source image and display result
	g_mainWindowP->preview();
}



// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BoxFilter::reset:
//
// Reset parameters.
//
void
BoxFilter::reset()
{
	m_slider[0]->setValue(3);
	m_slider[1]->setValue(3);
	m_checkBox->setCheckState(Qt::Checked);

	// apply filter to source image and display result
	g_mainWindowP->preview();
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BoxFilter::initShader:
//
// init shader program and parameters.
//
void
BoxFilter::initShader()
{
    m_nPasses = 1;
	// initialize GL function resolution for current context
	initializeGLFunctions();

	UniformMap uniforms;

	// init uniform hash table based on uniform variable names and location IDs
    uniforms["u_Wsize"   ] = WSIZE;//filter width
    uniforms["u_Hsize"   ] = HSIZE;//filter height
    uniforms["u_HStep"   ] = HSTEP;//horizontal step
    uniforms["u_VStep"   ] = VSTEP;//vertical step
    uniforms["u_Sampler" ] = SAMPLER;
    uniforms["u_filter"]   = FILTER;  //filter uniform location

	// compile shader, bind attribute vars, link shader, and initialize uniform var table
	g_mainWindowP->glw()->initShader(m_program[PASS1],
                     QString(":/hw2/vshader_boxfilter.glsl"),
                     QString(":/hw2/fshader_boxfilter.glsl"),
					 uniforms,
					 m_uniform[PASS1]);
	uniforms.clear();

	m_shaderFlag = true;
}


// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// BoxFilter::gpuProgram:
//
// Active BoxFilter gpu program
//
void
BoxFilter::gpuProgram(int pass)
{
    int w_size = m_slider[0]->value();
    int h_size = m_slider[1]->value();
    if(w_size % 2 == 0) ++w_size;
    if(h_size % 2 == 0) ++h_size;

    //compute a gaussian filter
    for(int i =0; i<w_size; ++i){
        for(int j=0;j<h_size;++j){
            //m_filter[i*w_size + j] = (float) exp(-(((i<<1 + j<<1) / (w_size <<1))*0.5)) / 2*PI;
            m_filter[i*w_size + j] = 1 / (float)(w_size * h_size);
        }
    }

    glUseProgram(m_program[PASS1].programId());

    //upload uniform values to the gpu
    glUniform1i (m_uniform[PASS1][WSIZE], w_size);
    glUniform1i (m_uniform[PASS1][HSIZE], h_size);
    glUniform1f (m_uniform[PASS1][HSTEP], (GLfloat) 1.0f / m_width);
    glUniform1f (m_uniform[PASS1][VSTEP], (GLfloat) 1.0f / m_height);
    glUniform1i (m_uniform[PASS1][SAMPLER], 0);

    //pass the filter array
    glUniform1fv(m_uniform[PASS1][FILTER],w_size*h_size,m_filter);

}
