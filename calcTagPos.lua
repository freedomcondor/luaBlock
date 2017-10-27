package.cpath = package.cpath .. ';../solvepnp/build/?.so'
require("libsolvepnp")
function calTagPos(tag)
	--for i = 1,4 do
	--	print("\t\ttagcorner",i,"x = ",tag.corners[i].x,"y = ",tag.corners[i].y) end

	res = libsolvepnp.solvepnp(tag.corners)
	print("\t\tin lua result: ",res.rotation.x)
	print("\t\tin lua result: ",res.rotation.y)
	print("\t\tin lua result: ",res.rotation.z)
	print("\t\tin lua result: ",res.translation.x)
	print("\t\tin lua result: ",res.translation.y)
	print("\t\tin lua result: ",res.translation.z)
	return res
end
