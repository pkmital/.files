unsigned int arc4random();
unsigned int arc4random_uniform(unsigned int upper_bound);
 
int earc4random()
{
	return arc4random();
}
 
int earc4random(int maxValue)
{
	return arc4random_uniform(maxValue);
}
 
int earc4random(int minValue, int maxValue)
{
	return arc4random_uniform(maxValue-minValue) + minValue;
}
