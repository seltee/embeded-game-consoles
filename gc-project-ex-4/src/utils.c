#include "utils.h"

const int _sins[] = {0, 2, 4, 7, 9, 11, 13, 16, 18, 20, 22, 24, 27, 29, 31, 33, 35, 37, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 73, 75, 77, 79, 81, 82, 84, 86, 87, 89, 91, 92, 94, 95, 97, 98, 99, 101, 102, 104, 105, 106, 107, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 119, 120, 121, 122, 122, 123, 124, 124, 125, 125, 126, 126, 126, 127, 127, 127, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 128, 127, 127, 127, 126, 126, 126, 125, 125, 124, 124, 123, 122, 122, 121, 120, 119, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 107, 106, 105, 104, 102, 101, 99, 98, 97, 95, 94, 92, 91, 89, 87, 86, 84, 82, 81, 79, 77, 75, 73, 72, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 37, 35, 33, 31, 29, 27, 24, 22, 20, 18, 16, 13, 11, 9, 7, 4, 2, 0, -2, -4, -7, -9, -11, -13, -16, -18, -20, -22, -24, -27, -29, -31, -33, -35, -37, -40, -42, -44, -46, -48, -50, -52, -54, -56, -58, -60, -62, -64, -66, -68, -70, -72, -73, -75, -77, -79, -81, -82, -84, -86, -87, -89, -91, -92, -94, -95, -97, -98, -99, -101, -102, -104, -105, -106, -107, -109, -110, -111, -112, -113, -114, -115, -116, -117, -118, -119, -119, -120, -121, -122, -122, -123, -124, -124, -125, -125, -126, -126, -126, -127, -127, -127, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -127, -127, -127, -126, -126, -126, -125, -125, -124, -124, -123, -122, -122, -121, -120, -119, -119, -118, -117, -116, -115, -114, -113, -112, -111, -110, -109, -107, -106, -105, -104, -102, -101, -99, -98, -97, -95, -94, -92, -91, -89, -87, -86, -84, -82, -81, -79, -77, -75, -73, -72, -70, -68, -66, -64, -62, -60, -58, -56, -54, -52, -50, -48, -46, -44, -42, -40, -37, -35, -33, -31, -29, -27, -24, -22, -20, -18, -16, -13, -11, -9, -7, -4, -2};
const int _coss[] = {128, 128, 128, 128, 128, 128, 127, 127, 127, 126, 126, 126, 125, 125, 124, 124, 123, 122, 122, 121, 120, 119, 119, 118, 117, 116, 115, 114, 113, 112, 111, 110, 109, 107, 106, 105, 104, 102, 101, 99, 98, 97, 95, 94, 92, 91, 89, 87, 86, 84, 82, 81, 79, 77, 75, 73, 72, 70, 68, 66, 64, 62, 60, 58, 56, 54, 52, 50, 48, 46, 44, 42, 40, 37, 35, 33, 31, 29, 27, 24, 22, 20, 18, 16, 13, 11, 9, 7, 4, 2, 0, -2, -4, -7, -9, -11, -13, -16, -18, -20, -22, -24, -27, -29, -31, -33, -35, -37, -40, -42, -44, -46, -48, -50, -52, -54, -56, -58, -60, -62, -64, -66, -68, -70, -72, -73, -75, -77, -79, -81, -82, -84, -86, -87, -89, -91, -92, -94, -95, -97, -98, -99, -101, -102, -104, -105, -106, -107, -109, -110, -111, -112, -113, -114, -115, -116, -117, -118, -119, -119, -120, -121, -122, -122, -123, -124, -124, -125, -125, -126, -126, -126, -127, -127, -127, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -128, -127, -127, -127, -126, -126, -126, -125, -125, -124, -124, -123, -122, -122, -121, -120, -119, -119, -118, -117, -116, -115, -114, -113, -112, -111, -110, -109, -107, -106, -105, -104, -102, -101, -99, -98, -97, -95, -94, -92, -91, -89, -87, -86, -84, -82, -81, -79, -77, -75, -73, -72, -70, -68, -66, -64, -62, -60, -58, -56, -54, -52, -50, -48, -46, -44, -42, -40, -37, -35, -33, -31, -29, -27, -24, -22, -20, -18, -16, -13, -11, -9, -7, -4, -2, 0, 2, 4, 7, 9, 11, 13, 16, 18, 20, 22, 24, 27, 29, 31, 33, 35, 37, 40, 42, 44, 46, 48, 50, 52, 54, 56, 58, 60, 62, 64, 66, 68, 70, 72, 73, 75, 77, 79, 81, 82, 84, 86, 87, 89, 91, 92, 94, 95, 97, 98, 99, 101, 102, 104, 105, 106, 107, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 119, 120, 121, 122, 122, 123, 124, 124, 125, 125, 126, 126, 126, 127, 127, 127, 128, 128, 128, 128, 128};