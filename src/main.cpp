#include <iostream>
#include <random>
#include <time.h>

template <class T, class func>
std::vector<T> eval(std::vector<T> range, func f)
{
  std::vector<T> ret(range.size());
  for (const auto& x : range)
    ret.push_back(f(x));
  return ret;
};

template <class func1, class func2>
struct compose_f
{
  compose_f(func1 f1, func2 f2) : f1(f1), f2(f2) {}
  
  template <class T>
  auto operator()(T x)                  { return f1(f2(x));           }
  template <class T>
  auto operator()(std::vector<T> range) { return eval(range, f1(f2)); }

private:
  func1 f1;
  func2 f2;
};

int main(int argc, char * argv[])
{
  double noise_mean = 0;
  double noise_sigma = 10;

  auto f = [] (double x)
	   {
	     return x;
	   };
  
  auto noise_f = [=] (double x)
	   {
	     std::default_random_engine gen;
	     gen.seed(time(0));
	     std::normal_distribution<double> n(noise_mean, noise_sigma);	    
	     return x + n(gen);
	   };

  compose_f<decltype(noise_f), decltype(f)> model_with_noise(noise_f, f);
	     
  std::cout << model_with_noise(100) << std::endl;
  return 0;
}
