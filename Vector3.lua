local Vector3 = {CLASS = "Vector3"}
Vector3.__index = Vector3

function Vector3:create(x,y,z)
	local instance = {}
	setmetatable(instance,self)
	self.__index = self
		--the metatable of instance would be whoever owns this create
	      --so you can :  a = State:create();  b = a:create();  grandfather-father-son
	-- Asserts
	if type(x) == "table" and x.CLASS == "Vector3" then
		instance.x = x.x
		instance.y = x.y
		instance.z = x.z
		return instance
	end
	if 	type(x) == "number" and
		type(y) == "number" and
		type(z) == "number" then
		instance.x = x
		instance.y = y
		instance.z = z
		return instance
	end
	
	-- add data
	instance.x = 0
	instance.y = 0
	instance.z = 0
	return instance
end

function Vector3.__add(a,b)
	local c = Vector3:create()
	c.x = a.x + b.x
	c.y = a.y + b.y
	c.z = a.z + b.z
	return c
end

function Vector3.__unm(b)
	local c = Vector3:create(-b.x,-b.y,-b.z)
	return c
end

function Vector3.__sub(a,b)
	local c = Vector3:create(-b.x,-b.y,-b.z)
	c = c + a
	return c
end

function Vector3.__mul(a,b)
	if 	type(a) == "table" and a.CLASS == "Vector3" and
		type(b) == "table" and b.CLASS == "Vector3" then
		local c = Vector3:create(	a.y * b.z - a.z * b.y,
									a.z * b.x - a.x * b.z,
									a.x * b.y - a.y * b.x
								)
		return c
	end
	if type(b) == "number" then
		local c = Vector3:create(a.x * b, a.y * b, a.z * b)
		return c
	end
	if type(a) == "number" then
		local c = Vector3:create(b.x * a, b.y * a, b.z * a)
		return c
	end
end

function Vector3.__div(a,b)
	if type(b) == "number" then
		return a * (1/b)
	end
	return nil
end

function Vector3.__pow(a,b)
	if type(b) == "number" then
		if b == 2 then
			local c = a ^ a
			return c
		else
			print("In Vector__pow:it doesn't mean anything")
			return nil
		end
	end
	if type(b) == "table" and b.CLASS == "Vector3" then
		return a.x * b.x + a.y * b.y + a.z * b.z
	end
end

function Vector3.__eq(a,b)
	return a.x == b.x and a.y == b.y and a.z == b.z
end

--function Vector3.__lt(a,b)  <
--function Vector3.__le(a,b)  <=

function Vector3:squlen()
	return self.x * self.x + self.y * self.y + self.z * self.z
end

function Vector3:len()
	return math.sqrt(self:squlen())
end

function Vector3:Nor()
	return Vector3:create(self.x / self:len(), self.y / self:len(), self.z / self:len())
end
--function normalize
--function angle axis

function Vector3:__tostring()
	return "(" .. self.x .. "," .. self.y .. "," .. self.z .. ")"
end

-- need to require Quaternion to use this:
function Vector3:rotate(q)
	if type(q) == "table" and q.CLASS == "Quaternion" then
		--self = Vec3:create(q:toRotate(self))
		return q:toRotate(self)
	else
		print("In Vector3:rotate, para not a Quaternion")
	end
end


return Vector3
