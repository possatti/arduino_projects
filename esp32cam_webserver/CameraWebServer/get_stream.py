import cv2

url = 'http://192.168.15.9:81/stream'
cap = cv2.VideoCapture(url)
while (True):
    ret,frame = cap.read()
    """
    your code here
    """
    cv2.imshow('frame',frame)
    if cv2.waitKey(20) & 0xFF == ord('q'):
        break    

cap.release()
cv2.destroyAllWindows()
