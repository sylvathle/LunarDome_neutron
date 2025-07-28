import sys


nsample = sys.argv[3]

txt_std_ar_thickness = sys.argv[2]
std_ar_thickness = float(sys.argv[2]) # g.cm-2

#"hf_pe"

densities = {
	"hf":13.31,
	"pe":0.94
}

densities["hfpe"] = 1.0 / ( 0.3/densities["hf"] + (1.0-0.3)/densities["pe"] )
print (densities)

materials = {
	"hf":"G4_Hf",
	"pe":"G4_POLYETHYLENE",
	"hfpe":"HfPE"
}

domeConfig = sys.argv[1]

list_ele = domeConfig.split("_")
n_ele = len(list_ele)

sum_densities = 0
for ele in list_ele:
  sum_densities += densities[ele]

macDir = str(txt_std_ar_thickness)+"-"+domeConfig


thick = std_ar_thickness/sum_densities 

f = open(macDir+".mac","w")


f.write("/SIM/scoring/phantom IcruSphere\n\n")

f.write("/run/initialize\n\n")

f.write("/SIM/scoring/sampleSize "+str(nsample)+"\n")
f.write("/SIM/scoring/resDir " +macDir+"\n\n")#rego30

f.write("/SIM/geometry/domeIn/radius 400\n")

n = 1
while n<=n_ele:
  print (list_ele[n-1])
  f.write("/SIM/geometry/dome"+str(n)+"/thick "+str(thick)+"\n")
  f.write("/SIM/geometry/dome"+str(n)+"/material "+materials[list_ele[n-1]]+"\n")
  n = n+1
 
f.write("/SIM/geometry/dome"+str(n)+"/thick 300.0\n")
f.write("/SIM/geometry/dome"+str(n)+"/material RegoAp17\n")
n = n+1

while n<=4:
  f.write("/SIM/geometry/dome"+str(n)+"/thick 1.0\n")
  f.write("/SIM/geometry/dome"+str(n)+"/material G4_Galactic\n")
  n = n+1

f.write("\n")
#f.write("/SIM/geometry/dome3/thick 1.0\n")
#f.write("/SIM/geometry/dome3/material G4_Galactic\n")
#f.write("/SIM/geometry/dome4/thick 1.0\n")
#f.write("/SIM/geometry/dome4/material G4_Galactic\n\n")

f.write("/SIM/scoring/radbeam 800.0 mm\n\n")

f.write("/run/beamOn "+str(nsample)+"\n")
