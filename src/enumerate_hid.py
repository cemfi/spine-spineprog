import hid

#Get the list of all devices matching this vendor_id/product_id
vendor_id = 0x2341
product_id = 0x8036

device_list = hid.enumerate(vendor_id, product_id)

for device in device_list:
    print(device)
