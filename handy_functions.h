int limit(int input, int min, int max) {
  if ( input > max ) return max;
  if ( input < min ) return min;
  return input;
}
bool inRange(float input, float min, float max) {
  if ( input > max ) return false;
  if ( input < min ) return false;
  return true;
}

float rescaleOffsetReference(float input, float input_min, float input_max, float input_reference, float output_min, float output_max) {
  return (input-input_reference) / (input_max-input_min) * (output_max-output_min) + output_min;
}
float rescaleOffset(float input, float input_min, float input_max, float output_min, float output_max) {
  return rescaleOffsetReference(input, input_min, input_max, input_min, output_min, output_max);
}
float rescale(float input, float input_range, float output_range) {
  return rescaleOffset(input, 0, input_range, 0, output_range);
}

String upperCaseStr(String str) {
  str.toUpperCase();
  return str;
}

