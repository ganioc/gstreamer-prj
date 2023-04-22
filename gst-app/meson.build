app_sources = [
  'src/main.c',
  'src/play.c'
  ]

airplane_sources = [
  'src/airplane.c',
  'src/airpipeline-macos.c',
  'src/airpipeline-linux.c'
]

executable('gst-app', app_sources, dependencies : [gst_dep])

executable('airplane', airplane_sources, dependencies: [gst_dep])