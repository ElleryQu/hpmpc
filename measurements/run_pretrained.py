import argparse
import os
import subprocess

# Export MODEL_DIR and DATA_DIR at the start of the script
os.environ["MODEL_DIR"] = "nn/Pygeon/models/pretrained"
os.environ["DATA_DIR"] = "nn/Pygeon/data/datasets"

adam_005_tests = [
    {"function": 180, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_005/AlexNet_32_CIFAR-10_custom_best.bin"},
    {"function": 180, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_005/AlexNet_32_CIFAR-10_standard_best.bin"},
    {"function": 181, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_005/AlexNet_CryptGPU_CIFAR-10_custom_best.bin"},
    {"function": 181, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_005/AlexNet_CryptGPU_CIFAR-10_standard_best.bin"},
    {"function": 170, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_005/ResNet18_avg_CIFAR-10_custom_best.bin"},
    {"function": 170, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_005/ResNet18_avg_CIFAR-10_standard_best.bin"},
    {"function": 171, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_005/ResNet50_avg_CryptGPU_CIFAR-10_custom_best.bin"},
    {"function": 171, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_005/ResNet50_avg_CryptGPU_CIFAR-10_standard_best.bin"},
    {"function": 172, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_005/ResNet101_avg_CIFAR-10_custom_best.bin"},
    {"function": 172, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_005/ResNet101_avg_CIFAR-10_standard_best.bin"},
    {"function": 173, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_005/ResNet152_avg_CryptGPU_CIFAR-10_custom_best.bin"},
    {"function": 173, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_005/ResNet152_avg_CryptGPU_CIFAR-10_standard_best.bin"},
    {"function": 174, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_005/VGG16_CIFAR-10_custom_best.bin"},
    {"function": 174, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_005/VGG16_CIFAR-10_standard_best.bin"},
    {"function": 182, "data_path": "MNIST_custom_test", "model_path": "Cifar_adam_005/LeNet_MNIST_custom_best.bin"},
    {"function": 182, "data_path": "MNIST_standard_test", "model_path": "Cifar_adam_005/LeNet_MNIST_standard_best.bin"}
]

adam_001_tests = [
    {"function": 180, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_001/AlexNet_32_CIFAR-10_custom_best.bin"},
    {"function": 180, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_001/AlexNet_32_CIFAR-10_standard_best.bin"},
    {"function": 181, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_001/AlexNet_CryptGPU_CIFAR-10_custom_best.bin"},
    {"function": 181, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_001/AlexNet_CryptGPU_CIFAR-10_standard_best.bin"},
    {"function": 170, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_001/ResNet18_avg_CIFAR-10_custom_best.bin"},
    {"function": 170, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_001/ResNet18_avg_CIFAR-10_standard_best.bin"},
    {"function": 171, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_001/ResNet50_avg_CryptGPU_CIFAR-10_custom_best.bin"},
    {"function": 171, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_001/ResNet50_avg_CryptGPU_CIFAR-10_standard_best.bin"},
    {"function": 172, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_001/ResNet101_avg_CIFAR-10_custom_best.bin"},
    {"function": 172, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_001/ResNet101_avg_CIFAR-10_standard_best.bin"},
    {"function": 173, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_001/ResNet152_avg_CryptGPU_CIFAR-10_custom_best.bin"},
    {"function": 173, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_001/ResNet152_avg_CryptGPU_CIFAR-10_standard_best.bin"},
    {"function": 174, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_adam_001/VGG16_CIFAR-10_custom_best.bin"},
    {"function": 174, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_adam_001/VGG16_CIFAR-10_standard_best.bin"},
    {"function": 182, "data_path": "MNIST_custom_test", "model_path": "Cifar_adam_001/LeNet_MNIST_custom_best.bin"},
    {"function": 182, "data_path": "MNIST_standard_test", "model_path": "Cifar_adam_001/LeNet_MNIST_standard_best.bin"}
]

sgd_001_tests = [
    {"function": 180, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_sgd_001/AlexNet_32_CIFAR-10_custom_best.bin"},
    {"function": 180, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_sgd_001/AlexNet_32_CIFAR-10_standard_best.bin"},
    {"function": 181, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_sgd_001/AlexNet_CryptGPU_CIFAR-10_custom_best.bin"},
    {"function": 181, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_sgd_001/AlexNet_CryptGPU_CIFAR-10_standard_best.bin"},
    {"function": 170, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_sgd_001/ResNet18_avg_CIFAR-10_custom_best.bin"},
    {"function": 170, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_sgd_001/ResNet18_avg_CIFAR-10_standard_best.bin"},
    {"function": 171, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_sgd_001/ResNet50_avg_CryptGPU_CIFAR-10_custom_best.bin"},
    {"function": 171, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_sgd_001/ResNet50_avg_CryptGPU_CIFAR-10_standard_best.bin"},
    {"function": 172, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_sgd_001/ResNet101_avg_CIFAR-10_custom_best.bin"},
    {"function": 172, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_sgd_001/ResNet101_avg_CIFAR-10_standard_best.bin"},
    {"function": 173, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_sgd_001/ResNet152_avg_CryptGPU_CIFAR-10_custom_best.bin"},
    {"function": 173, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_sgd_001/ResNet152_avg_CryptGPU_CIFAR-10_standard_best.bin"},
    {"function": 174, "data_path": "CIFAR-10_custom_test", "model_path": "Cifar_sgd_001/VGG16_CIFAR-10_custom_best.bin"},
    {"function": 174, "data_path": "CIFAR-10_standard_test", "model_path": "Cifar_sgd_001/VGG16_CIFAR-10_standard_best.bin"},
    {"function": 182, "data_path": "MNIST_custom_test", "model_path": "Cifar_sgd_001/LeNet_MNIST_custom_best.bin"},
    {"function": 182, "data_path": "MNIST_standard_test", "model_path": "Cifar_sgd_001/LeNet_MNIST_standard_best.bin"}
]

tests = [adam_005_tests, adam_001_tests, sgd_001_tests]

def run_command(command):
    subprocess.run(command, shell=True, check=True)

def main():
    parser = argparse.ArgumentParser(description='Run configurations')
    parser.add_argument('-p', type=str, default='all', help='Party identifier')
    parser.add_argument('-a', type=str, default='127.0.0.1', help='IP address for a')
    parser.add_argument('-b', type=str, default='127.0.0.1', help='IP address for b')
    parser.add_argument('-c', type=str, default='127.0.0.1', help='IP address for c')
    parser.add_argument('-d', type=str, default='127.0.0.1', help='IP address for d')
    parser.add_argument('-i', type=int, default=1, help='Number of random seeds per run')
    parser.add_argument('-n', type=int, default=100, help='Number of images to use for testing')

    args = parser.parse_args()
    args.i = str(list(range(args.i))).strip('[]').replace(', ', ',')


    for test_group in tests:
        for setup in test_group:
            # Set environment variables
            os.environ["MODEL_FILE"] = f"{setup['model_path']}"
            os.environ["SAMPLES_FILE"] = f"{setup['data_path']}_images.bin"
            os.environ["LABELS_FILE"] = f"{setup['data_path']}_labels.bin"

            # Prepare the base command
            base_cmd = f"python measurements/run_config.py -p {args.p} -a {args.a} -b {args.b} -c {args.c} -d {args.d}"

            # Run commands for different configurations
            configs = [
                "measurements/configs/nn_tests/test_fractional_vs_accuracy_16.conf",
                "measurements/configs/nn_tests/test_fractional_vs_accuracy_32.conf",
                "measurements/configs/nn_tests/test_fractional_vs_accuracy_64.conf"
            ]

            for config in configs:
                print(f"FUNCTION_IDENTIFIER={setup['function']} SRNG_SEED={args.i} NUM_INPUTS={args.n}")
                cmd = f"{base_cmd} {config} --override FUNCTION_IDENTIFIER={setup['function']} SRNG_SEED={args.i} NUM_INPUTS={args.n}"
                run_command(cmd)
if __name__ == "__main__":
    main()
