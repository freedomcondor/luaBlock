package.cpath = package.cpath .. ';../solvepnp/build/?.so'
require("libsolvepnp")
function calTagPos(tag)
	--for i = 1,4 do
	--	print("\t\ttagcorner",i,"x = ",tag.corners[i].x,"y = ",tag.corners[i].y) end

	res = libsolvepnp.solvepnp(tag.corners)
	x = res.rotation.x
	y = res.rotation.y
	z = res.rotation.z
	th = math.sqrt(x * x + y * y + z * z)

	--print(math.sin(0.5 * math.pi))
	a = x / th
	b = y / th
	c = z / th
	d = math.cos(th/2)
	-- print(a * a + b * b + c * c)  -- should be 1
 	a = a * math.sin(th/2)
 	b = b * math.sin(th/2)
 	c = c * math.sin(th/2)

	res.rotation.x = 2 * (a * c + b * d)
	res.rotation.y = 2 * (b * c - a * d)
	res.rotation.z = 1 - 2 * (a * a + b * b)

	--[[
	print("\t\tin lua result: ",res.rotation.x)
	print("\t\tin lua result: ",res.rotation.y)
	print("\t\tin lua result: ",res.rotation.z)
	print("\t\tin lua result: ",res.translation.x)
	print("\t\tin lua result: ",res.translation.y)
	print("\t\tin lua result: ",res.translation.z)
	--]]
	return res
end
