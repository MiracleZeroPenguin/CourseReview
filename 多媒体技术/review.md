## 3 Graphics and Image Data Representation

* Categories of Images
  * 1-Bit Images, 8-Bit Grey-Level Images, 24-Bit Color Images
  * Definitions, Storing space, and Usage
  * The feature of 32-bit color image
* 8-Bit Color Images
  * Concept and features
  * Applications
* LUTs(Color Lookup Tables)
* Dithering Algorithm
  * an $N*N$ matrix represents $N^2+1$ levels of intensity
  * 打印机

## 4 Color in Image and Video

* Gamma Correction

  * Concept
  * Method: $R\to R'=R1/\gamma\to\to (R')\gamma\to R$

* Other color models

  * HSL(HSB)——hue, Saturation, Lightness/Brightness
  * HSV——Hue Saturation Value
  * HIS——Hue, Saturation and Intensity
  * HCI——C=Chroma
  * HVC——V=valu

* Color Models in Images

  * RGB color model for CRT Display
  * CMY color model
  * Transformation from RBG to CMY

* Color Models in Video

  * YUV, YIQ, YCbCr

  * Definition and application

  * Relationship of various models
    $$
    Y=0.299R+0.587G+0.114B\\
    \text{Chrominance as:}-U=B-Y;V=R-Y\\
    I=0.736(R-Y)-0.268(B-Y)\\
    Q=0.478(R-Y)+0.413(B-Y)\\
    Cb=(B-Y)/1.772+0.5\\
    Cr=(R-Y)/1.402+0.5
    $$

## 5 Fundamental Concepts in Video

* Types of Video Signals
  * Component video, Composite Video, s-Video
  * Concepts and Usages
* Analog Video
  * Related Concepts
    * Progressive and interlace scanning
    * vertical retrace and horizontal retrace
    * Field and frame
  * NTSC, PAL and SECAM
  * How to encoding and decoding the composite video signal
  * Chroma subsampling
* Digital video CCIR standard
  * NTSC standard
    * 525 lines; 858 pixels(where, 720 is visible)
    * 4:2:2 schema
    * one pixel——2bytes
  * 4CIF/CIF/QCIF/SQCIF
  * HDTV

## 6 Basics of Digital Audio

* Quantization

  * Sampling in the amplitude dimension
    * Uniform sampling
    * Nonuniform sampling

* Nyquist Theorem

  * Nyquist rate
    * For correct sampling, sampling rate must be at least twice the maximum frequency content in the signal
    * Nyquist frequency: Half the Nyquist rate

* Some basic concepts

  * SNR
  * SQNR
  * Linear and Nonlinear Quantization
    * U-Law; A-Law

* Coding of Audio

  * PCM, Pulse Code Modulation
  * DPCM, Difference version of PCM
  * ADPCM, Adaptive DPCM

* How to calculate the bit-rate of coded audio signal

  采样频率48kHz，量化位数32位，4声道：每个采样点4*32=128bit，每秒48000个采样点，bit-rate为128\*48000 bit/sec

* PCM signal encoding and decoding

* DPCM workflow

## 7 Lossless Compression Algorithms

* Basics of Information Theory
  * The entropy of an information source(熵)
    * Alphabet S={s1,s2,...,sn}
    * $\eta=H(S)$
    * $-\Sigma_{i=1}^n p_i\log_2p_i$
  * Self-information: $\log_2{\frac{1}{P_i}}$
  * The concept of entropy
    * A measure of the disorder of a system
    * The more entropy, the more disorder
* Identifying often-occurring symbols as short codewords
* Lossless Coding Algorithms
  * RLC(Run-Length Coding)
  * Variable-Length Coding
    * Shannon-Fano algorithm
    * Huffman Coding
    * Adaptive Huffman Coding
  * Dictionary-Based Coding
  * Arithmetic Coding
* Lossless Image Compression
  * Differential Coding
  * Lossless JPEG

## 8 Lossy Compression Algorithms

* The Rate-Distortion Theory
  * Distortion Measures
    * MSE, SNR, PSNR
  * The concept of R(D)
* Quantization: The heart of any lossy sheme
  * Nonuniform Scaler Quantization
    * The LIoyd-Max Quantizer
    * The companded Quantize
  * Uniform Scalar Quantization
  * Two types quantizers: midrise and midtread
* Transform Coding
  * 1D DCT
  * 2D DCT

## 9 Image Compression Standards

* Main Steps in JPEG Image Compression
* JPEG Modes
  * Sequential Mode
  * Progressive Mode
  * Hierarchical Mode
  * Lossless Mode
* JPEG Bitstream

## 10 Basic Video Compression Techniques

* Basic Concept
  * Temporal redundancy: Predictive Coding
    * Subtract images in time order
    * Code the residual error
  * Better methods:
    * Search for the right parts
    * Motion Estimation
    * Motion Compensation
* Video Compression Based on Motion Compensation
  * Reduce spatial redundancy and temporal redundancy
  * Intra-Frame: similar as JPEG
  * Inter-Frame: based on motion prediction and compensation
* The three main steps
  * Motion estimation: motion vector search
  * Motion-compensation: based prediction
  * Derivation of the prediction error

* Search For Motion Vectores

  * Criteria of matching($MAD(i,j)$)
  * Sequential Search
  * 2D-Logarithmic-search
  * Hierarchical Search

* H.261-Overview

  * 2 video format: QCIF, CIF
  * 2 type frame image: I-Frame, P-Frame
  * Coding method: Intra-frame, inter-frame

* Quantization in H.261

  * Constant Step-size in the range $[2,62]$ instead of 8*8matrixes
    $$
    QDCD=\lfloor\frac{DCT}{step\_size}\rfloor=\lfloor\frac{DCT}{2\times scale}\rfloor
    $$

  * Intra-frame, for DC, stepsize=8

* H.263-overview

  * sub-QCIF, 4CIF and 16CIF
  * GOBs don't have a fixed size
  * half-pixel positions

* Optional H.263 Coding Modes

  * Unrestricted motion vector mode
  * Syntax-based arithmetic coding mode
  * Advanced prediction mode(4 MV for a macroblock)
  * PB-frames mode

## 11 MPEG Video Coding

* MPEG-1-Overview
  * Support only non-interlaced Video
  * 352*240 for NTSC at 30 fps
  * 352*288 for PAL at 25 fps
  * 4:2:0 chroma subsampling
* Difference from H.261
  * B frames——Bidirectional Motion Compensation
  * Format specifications
  * Picture can be divided into 1 or more slices
  * Different quantization tables for intra- and inter- coding
  * Motion vectors to be of 0.5pixel
  * Larger gaps between I and P frames and larger motion-vector search range

* MEPG-2 Scalabilities
  * SNR
  * Spatial
  * Temporal
  * Data
* Other Major difference from MPEG-1
  * Better resilience to errors
  * Support to 4:2:2 and 4:$:4 chroma subsampling
  * Nonlinear restricted structure
  * More restricted slice structure
  * More flexible video formats
* MPEG-4-Overview
  * User interactivities
  * A large range between 5kbps and 10Mbps
  * Some characteristics
    * Motion Compensation
    * Object based coding
    * Arbitrary Shape Coding
    * Static texture coding
    * Face object coding and Animation
    * Body object coding and Animation