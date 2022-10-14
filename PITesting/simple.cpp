#include <unistd.h>
#include <PCA9685.h>

int main(){
	PiPCA9685::PCA9685 pca {};

	while(true){
		pca.set_pwm_pow(0, .5);
		usleep(1'000'000);
		pca.set_pwm_pow(0, 0);
		pca.set_pwm_pow(1, .5);
		usleep(1'000'000);
		pca.set_pwm_pow(1, 0);
		pca.set_pwm_pow(2, .5);
		usleep(1'000'000);
		pca.set_pwm_pow(2, 0);
		pca.set_pwm_pow(3, .5);
		usleep(1'000'000);
		pca.set_pwm_pow(3, 0);
	}
}