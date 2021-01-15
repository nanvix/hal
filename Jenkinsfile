pipeline {
    agent {
		docker {
			image : 'nanvix/ubuntu:unix64'
			label : 'jenkins-hal-unix'
			args  : '--rm --privileged -v "$(pwd):/mnt" -p 3456:3456'
		}
	}

	environment {
		setup='bash utils/nanvix-setup-unix.sh'
		dbuild='make contrib VERBOSE=true && make all VERBOSE=true && chmod -R 777 .'
		rbuild='make contrib VERBOSE=true RELEASE=true && make all VERBOSE=true RELEASE=true && chmod -R 777 .'
		run='make test && chmod -R 777 . && chmod 664 nanvix-cluster-*'	
	}

    stages {
        stage('unix64') {
    		stages {
        		stage('Build Debug') {
					steps {
						lock('unix64')
						{
							echo 'Building..'
							sh 'ls'
						}
					}
				}
			}
        }
        stage('Build Release') {
			echo '1 ----'
        }
        stage('Run Debug') {
			echo '2 ----'
        }
        stage('Run Release') {
			echo '3 ----'
        }
    }
}
pipeline {
    parameters {
        choice(name: 'PLATFORM_FILTER', choices: ['all', 'mppa256', 'unix64', 'openrisc', 'riscv32'], description: 'Run on specific platform')
    }
    agent none
    stages {
        stage('Build Debug and Release') {
            matrix {
                agent {
                    label "${PLATFORM}-agent"
                }
                when { anyOf {
                    expression { params.PLATFORM_FILTER == 'all' }
                    expression { params.PLATFORM_FILTER == env.PLATFORM }
                } }
                axes {
                    axis {
                        name 'PLATFORM'
                        values 'mppa256', 'unix64', 'openrisc', 'riscv32'
                    }
                }
                excludes {
                    exclude {
                        axis {
                            name 'PLATFORM'
                            values 'linux'
                        }
                        axis {
                            name 'BROWSER'
                            values 'safari'
                        }
                    }
                    exclude {
                        axis {
                            name 'PLATFORM'
                            notValues 'windows'
                        }
                        axis {
                            name 'BROWSER'
                            values 'edge'
                        }
                    }
                }
                stages {
                    stage('Build') {
                        steps {
                            echo "Do Build for ${PLATFORM} - ${BROWSER}"
                        }
                    }
                    stage('Test') {
                        steps {
                            echo "Do Test for ${PLATFORM} - ${BROWSER}"
                        }
                    }
                }
            }
        }
    }
}
