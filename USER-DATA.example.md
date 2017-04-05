# USER-DATA EXAMPLE

_... Ensure this container runs on CoreOS when expected i.e. before other custom containers_


        coreos:
          units:
            - name: "coreos_aws_hostcfg"
              command: "start"
              content: |
                [Unit]
                Description=Provide utility scripts, services for aws host
                After=docker.service docker.socket network-online.target
                Requires=docker.service docker.socket network-online.target

                [Service]
				Type=oneshot
				RemainAfterExit=yes
				TimeoutSec=60
				Environment="_C=coreos_aws_hostcfg"
				Environment="_DI=opsgang/coreos_aws_hostcfg:stable"
				EnvironmentFile=-/etc/custom/docker_image_versions
				ExecStartPre=-/usr/bin/docker kill ${_C}
				ExecStartPre=-/usr/bin/docker rm ${_C}
				ExecStartPre=/usr/bin/docker pull ${_DI}
				ExecStart=/bin/bash -c " \
				docker run                                     \
					--name ${_C}                               \
					--privileged                               \
					-v /home/core:/home/core                   \
					-v /etc/custom:/etc/custom                 \
					-v /etc/coreos:/etc/coreos                 \
					-v /etc/systemd/system:/etc/systemd/system \
					${_DI} /bin/bash /home/core/bin/instance_info"
				ExecStop=-/usr/bin/docker kill ${_C}
				ExecStopPost=-/usr/bin/docker rm -f ${_C}

				[Install]
				WantedBy=multi-user.target

