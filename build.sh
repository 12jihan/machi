#!/bin/bash

# Colors for better output
RED='\033[0;31m'
GREEN='\033[0;32m'
YELLOW='\033[1;33m'
BLUE='\033[0;34m'
NC='\033[0m' # No Color

# Function to print colored messages
print_info() {
  echo -e "${BLUE}[INFO]${NC} $1"
}

print_success() {
  echo -e "${GREEN}[SUCCESS]${NC} $1"
}

print_warning() {
  echo -e "${YELLOW}[WARNING]${NC} $1"
}

print_error() {
  echo -e "${RED}[ERROR]${NC} $1"
}

# Check if CMakeLists.txt exists
if [ ! -f "CMakeLists.txt" ]; then
  print_error "CMakeLists.txt not found in current directory!"
  print_info "Make sure you're running this script from your project root directory."
  exit 1
fi

# Parse command line arguments
BUILD_TYPE="Release"
CLEAN_BUILD=false
RUN_AFTER_BUILD=true

while [[ $# -gt 0 ]]; do
  case $1 in
  -d | --debug)
    BUILD_TYPE="Debug"
    shift
    ;;
  -c | --clean)
    CLEAN_BUILD=true
    shift
    ;;
  --no-run)
    RUN_AFTER_BUILD=false
    shift
    ;;
  -h | --help)
    echo "Usage: $0 [OPTIONS]"
    echo "Options:"
    echo "  -d, --debug     Build in debug mode (default: Release)"
    echo "  -c, --clean     Clean build directory before building"
    echo "  --no-run        Don't run the application after building"
    echo "  -h, --help      Show this help message"
    exit 0
    ;;
  *)
    print_error "Unknown option: $1"
    echo "Use -h or --help for usage information"
    exit 1
    ;;
  esac
done

print_info "Starting build process..."
print_info "Build type: $BUILD_TYPE"

# Clean build directory if requested
if [ "$CLEAN_BUILD" = true ]; then
  print_warning "Cleaning build directory..."
  rm -rf build/
fi

# Create build directory if it doesn't exist
if [ ! -d "build" ]; then
  print_info "Creating build directory..."
  mkdir build
fi

# Change to build directory
cd build || {
  print_error "Failed to enter build directory!"
  exit 1
}

# Run CMake configuration
print_info "Configuring project with CMake..."
if ! cmake -DCMAKE_BUILD_TYPE=$BUILD_TYPE ..; then
  print_error "CMake configuration failed!"
  exit 1
fi

print_success "CMake configuration completed successfully!"

# Build the project
print_info "Building project..."
if ! cmake --build . --config $BUILD_TYPE; then
  print_error "Build failed!"
  exit 1
fi

print_success "Build completed successfully!"

# Find the executable
EXECUTABLE=""
if [ -f "bin/machi" ]; then
  EXECUTABLE="bin/machi"
elif [ -f "machi" ]; then
  EXECUTABLE="machi"
else
  print_error "Could not find the executable!"
  print_info "Looking for files in build directory:"
  find . -name "machi*" -type f
  exit 1
fi

print_success "Executable found: $EXECUTABLE"

# Run the application if requested
if [ "$RUN_AFTER_BUILD" = true ]; then
  print_info "Running application..."
  echo "----------------------------------------"
  ./$EXECUTABLE
  echo "----------------------------------------"
  print_success "Application finished running!"
else
  print_info "Skipping application run (--no-run flag was used)"
  print_info "To run manually: cd build && ./$EXECUTABLE"
fi
